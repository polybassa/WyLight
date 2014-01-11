package biz.bruenn.WyLight;

import java.util.concurrent.atomic.AtomicBoolean;

import biz.bruenn.WiflyLight.R;
import biz.bruenn.WyLight.view.ColorView;

import android.graphics.Color;
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

	private ColorView mColorStatus;
	private ImageView mCrosshair;
	private ImageView mColorPicker;

	/**
	 * Convert the point in the parent view to a HSV representation. hsv[0] is Hue [0 .. 360) hsv[1] is Saturation [0...1] hsv[2] is Value [0...1] and always 1
	 * @param x coordinate in the parent view
	 * @param y coordinate in the parent view
	 * @param hsv 3 element array which holds the resulting HSV components.
	 */
	private void coordinateToHSV(float x, float y, float[] hsv) {
		// translate center of the parent view to (0/0)
		final double x0 = Math.max(0, Math.min(mDiameter-1, x)) - mRadius;
		final double y0 = Math.max(0, Math.min(mDiameter-1, y)) - mRadius;
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

	public void onColorChanged(int color) {
		mColorStatus.setColor(color);
		setCrosshair(color);
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup group, Bundle savedInstanceState) {
		View v = inflater.inflate(R.layout.fragment_set_color, group, false);
		v.getViewTreeObserver().addOnGlobalLayoutListener(this);
		mColorStatus = (ColorView)v.findViewById(R.id.colorStatus);
		mCrosshair = (ImageView)v.findViewById(R.id.crosshair);
		mColorPicker = (ImageView)v.findViewById(R.id.colorPicker);
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
		mColorPicker.setOnTouchListener(new View.OnTouchListener() {
			private AtomicBoolean mChangeIsInProgress = new AtomicBoolean(false);
			
			public boolean onTouch(View v, MotionEvent event) {
				if(!mChangeIsInProgress.getAndSet(true)) {
					final float[] hsv = new float[3];
					coordinateToHSV(event.getX(), event.getY(), hsv);

					final int color = Color.HSVToColor(hsv);
					setColor(color);
					mChangeIsInProgress.set(false);
				}
				return true;
			}
		});
		mProvider.addOnColorChangedListener(this);
	}

	private void setCrosshair(int color) {
		final float[] hsv = new float[3];
		Color.colorToHSV(color, hsv);

		final double hue = hsv[0] * PI_DIVIDED_MINUS_180;
		final double saturation = hsv[1] * mRadius;
		final double x0 = Math.cos(hue) * saturation;
		final double y0 = Math.sin(hue) * saturation;
		mCrosshair.setX(mCenterX + (float)x0);
		mCrosshair.setY(mCenterY + (float)y0);
	}
}
