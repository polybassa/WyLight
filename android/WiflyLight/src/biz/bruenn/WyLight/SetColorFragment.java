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
			if(v.equals(mColorPicker)) {
				if(!mChangeIsInProgress.getAndSet(true)) {
					int x = Math.max(0, Math.min(mDiameter-1, (int)event.getX()));
					int y = Math.max(0, Math.min(mDiameter-1, (int)event.getY()));
					double x0 = x - mRadius;
					double y0 = y - mRadius;
					final double factor = (double)mRadius / Math.hypot(x0, y0);
					if (factor < 1d) {
						x0 *= factor;
						y0 *= factor;
						x = (int) Math.round(x0 + mRadius);
						y = (int) Math.round(y0 + mRadius);
					}

					final float[] hsv = new float[3];
					coordinateToHSV(v.getX() + x - mRadiusCrosshair, v.getY() + y - mRadiusCrosshair, hsv);
					mProvider.setColorHueSaturation(hsv[0], hsv[1]);
					mChangeIsInProgress.set(false);
				}
			} else if (v.equals(mValuePicker)) {
				if(!mChangeIsInProgress.getAndSet(true)) {
					float z = Math.max(v.getX(), Math.min(v.getX() + v.getWidth(), event.getX()));
					mProvider.setColorValue(coordinateToValue(z - mRadiusCrosshair));
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

	private float coordinateToValue(float z) {
		final float crosshairCenterZ = z + this.mRadiusCrosshair;
		return (crosshairCenterZ - mValuePicker.getX()) / (float)mValuePicker.getWidth();
	}

	@Override
	public int getIcon() {
		return R.drawable.ic_action_location_searching;
	}

	public void onColorChanged(float[] hsv) {
		if(null != mColorStatus) {
			mColorStatus.setColor(Color.HSVToColor(hsv));
			setCrosshair(hsv);
			setValueGradient(hsv.clone());
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
		final int shift = (mColorPicker.getWidth() - mCrosshair.getWidth()) / 2;
		mCenterX = mColorPicker.getX() + shift;
		mCenterY = mColorPicker.getY() + shift;
		mDiameter = mColorPicker.getWidth();
		mRadius = mDiameter / 2;
		mRadiusCrosshair = mCrosshair.getWidth() / 2;
		mColorPicker.setOnTouchListener(mOnTouchListener);
		mValuePicker.setOnTouchListener(mOnTouchListener);
		mProvider.addOnColorChangedListener(this);
	}

	private void setCrosshair(float[] hsv) {
		final double hue = hsv[0] * PI_DIVIDED_MINUS_180;
		final double saturation = hsv[1] * mRadius;
		final float value = hsv[2] * mValuePicker.getWidth();
		final double x0 = Math.cos(hue) * saturation;
		final double y0 = Math.sin(hue) * saturation;
		mCrosshair.setX(mCenterX + (float)x0);
		mCrosshair.setY(mCenterY + (float)y0);
		mValueCrosshair.setX(mValuePicker.getX() + value - mRadiusCrosshair);
	}

	private void setValueGradient(float[] hsv) {
		hsv[2] = 1f;
		ShapeDrawable mDrawable = new ShapeDrawable(new RectShape());
		LinearGradient gradient = new LinearGradient(0, 0, mValuePicker.getWidth(), 0, Color.BLACK, Color.HSVToColor(hsv), Shader.TileMode.CLAMP);
		mDrawable.getPaint().setShader(gradient);
		mValuePicker.setBackgroundDrawable(mDrawable);
	}
}
