package biz.bruenn.WyLight;

import java.util.concurrent.atomic.AtomicBoolean;

import biz.bruenn.WiflyLight.R;
import biz.bruenn.WyLight.view.ColorView;

import android.graphics.Color;
import android.graphics.LinearGradient;
import android.graphics.Shader;
import android.graphics.drawable.ShapeDrawable;
import android.graphics.drawable.shapes.RectShape;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewTreeObserver;
import android.widget.ImageView;

public class SetColorFragment extends ControlFragment implements OnColorChangeListener, ViewTreeObserver.OnGlobalLayoutListener {

	/**
	 * Constant to convert angle to radian measure
	 */
	private static final double PI_DIVIDED_MINUS_180 = Math.PI / -180;

	/**
	 * Constant to convert radian measure to angle
	 */
	private static final double _180_DIVIDED_PI = 180 / Math.PI;

	// These values only have to be recalculated, if the views layout changes
	private float mCenterX = 0;
	private float mCenterY = 0;
	private float mCenterZ = 0;
	private int mDiameter = 0;
	private int mRadius = 0;
	private int mRadiusCrosshair = 0;

	private ColorView mColorStatus;
	private ImageView mCrosshair;
	private ImageView mColorPicker;
	private View mValuePicker;
	private ImageView mValueCrosshair;
	private AtomicBoolean mChangeIsInProgress = new AtomicBoolean(false);

	private View.OnTouchListener mOnTouchListener = new View.OnTouchListener() {
		public boolean onTouch(View v, MotionEvent event) {
			if(event.getAction() != MotionEvent.ACTION_MOVE) {
				return true;
			}
			if(v.equals(mColorPicker)) {
				if(!mChangeIsInProgress.getAndSet(true)) {
					int x = Math.max(0, Math.min(mDiameter-1, (int)event.getX()));
					int y = Math.max(0, Math.min(mDiameter-1, (int)event.getY()));
					final int x0 = x - mRadius;
					final int y0 = y - mRadius;
					final float saturation = (float)Math.min(1d, Math.hypot(x0, y0) / mRadius);

					final double x1 = x - v.getWidth()/2;
					final double y1 = y - v.getWidth()/2;

					final double magnitude = Math.hypot(x1, y1);
					final double alpha = _180_DIVIDED_PI * Math.acos(x1/magnitude);
					final float hue = (float)((y1 < 0) ? alpha : 360 - alpha);

					mProvider.setColorHueSaturation(hue, saturation);
					mChangeIsInProgress.set(false);
				}
			} else if (v.equals(mValuePicker)) {
				if(!mChangeIsInProgress.getAndSet(true)) {
					final float value = Math.max(0f, Math.min(1f, event.getX() / v.getWidth()));
					mProvider.setColorValue(value);
					mChangeIsInProgress.set(false);
				}
			} else {
				return false;
			}
			return true;
		}
	};

	/**
	 * Convert the point in the parent view to a HSV representation. hsv[0] is Hue [0 .. 360) hsv[1] is Saturation [0...1] hsv[2] is Value [0...1] and always 1
	 * @param x coordinate in the parent view
	 * @param y coordinate in the parent view
	 * @param hsv 3 element array which holds the resulting HSV components.
	 */
	private void coordinateToHSV(float x, float y, float[] hsv) {
		// translate center of the parent view to (0/0)
		final double x0 = x - mCenterX;
		final double y0 = y - mCenterY;
		final double magnitude = Math.hypot(x0, y0);
		final double alpha = _180_DIVIDED_PI * Math.acos(x0/magnitude);
		hsv[0] = (float)((y0 < 0) ? alpha : 360 - alpha);
		hsv[1] = (float) Math.min(1d, magnitude/mRadius);
		hsv[2] = 1f;
	}

	@Override
	public int getIcon() {
		return R.drawable.ic_action_location_searching;
	}

	public void onColorChanged(float[] hsv) {
		if(null != mColorStatus) {
			mColorStatus.setColor(Color.HSVToColor(hsv));
			setCrosshairs(hsv);
			setValueGradient(hsv);
		}
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup group, Bundle savedInstanceState) {
		View v = inflater.inflate(R.layout.fragment_set_color, group, false);
		v.getViewTreeObserver().addOnGlobalLayoutListener(this);
		mColorStatus = (ColorView)v.findViewById(R.id.colorStatus);
		mCrosshair = (ImageView)v.findViewById(R.id.crosshair);
		mColorPicker = (ImageView)v.findViewById(R.id.colorPicker);
		mValuePicker = (View)v.findViewById(R.id.valuePicker);
		mValueCrosshair = (ImageView)v.findViewById(R.id.valueCrosshair);
		return v;
	}

	@Override
	public void onDetach() {
		super.onDetach();
	}

	public void onGlobalLayout() {
		if(this.isDetached()) {
			mProvider.removeOnColorChangedListener(this);
			return;
		}
		mDiameter = mColorPicker.getWidth();
		mRadius = mDiameter / 2;
		mRadiusCrosshair = mCrosshair.getWidth() / 2;
		final int shift = (mColorPicker.getWidth() - mCrosshair.getWidth()) / 2;
		mCenterX = mColorPicker.getX() + shift;
		mCenterY = mColorPicker.getY() + shift;
		mCenterZ = mValuePicker.getX() -mRadiusCrosshair;
		mColorPicker.setOnTouchListener(mOnTouchListener);
		mValuePicker.setOnTouchListener(mOnTouchListener);
		mProvider.addOnColorChangedListener(this);
	}

	private void setCrosshairs(float[] hsv) {
		final double hue = hsv[0] * PI_DIVIDED_MINUS_180;
		final double saturation = hsv[1] * mRadius;
		final float value = hsv[2] * mValuePicker.getWidth();
		final double x0 = Math.cos(hue) * saturation;
		final double y0 = Math.sin(hue) * saturation;
		mCrosshair.setX(mCenterX + (float)x0);
		mCrosshair.setY(mCenterY + (float)y0);
		mValueCrosshair.setX(mCenterZ + value);
	}

	/**
	 * updates the hsv value pickers background gradient
	 * @param hsv should be an array of three floats: hsv[0] is Hue [0 .. 360) hsv[1] is Saturation [0...1] hsv[2] is Value [0...1]
	 */
	private void setValueGradient(float[] ref) {
		final float[] hsv = new float[]{ref[0], ref[1], 1f};
		ShapeDrawable background = new ShapeDrawable(new RectShape());
		LinearGradient gradient = new LinearGradient(0, 0, mValuePicker.getWidth(), 0, Color.BLACK, Color.HSVToColor(hsv), Shader.TileMode.CLAMP);
		background.getPaint().setShader(gradient);
		mValuePicker.setBackgroundDrawable(background);
	}
}
