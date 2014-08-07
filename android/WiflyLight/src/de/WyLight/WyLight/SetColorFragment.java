/**
                Copyright (C) 2012 - 2014 Patrick Bruenn.

    This file is part of WyLight.

    Wylight is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    WiflyLight is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with WiflyLight.  If not, see <http://www.gnu.org/licenses/>. */

package de.WyLight.WyLight;

import java.util.concurrent.atomic.AtomicBoolean;

import de.WyLight.WyLight.R;
import de.WyLight.WyLight.view.ColorView;

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

public class SetColorFragment extends ControlFragment implements
		OnColorChangeListener, ViewTreeObserver.OnGlobalLayoutListener {

	/**
	 * Constant to convert angle to radian measure
	 */
	private static final double PI_DIVIDED_MINUS_180 = Math.PI / -180;

	/**
	 * Constant to convert radian measure to angle
	 */
	private static final double _180_DIVIDED_PI = 180 / Math.PI;

	/**
	 * Table of color button ids, which is used to easily connect them all with
	 * an OnClickListener.
	 */
	private static final int COLOR_BUTTONS[] = { R.id.red, R.id.yellow,
			R.id.green, R.id.cyan, R.id.blue, R.id.magenta };

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

	private final View.OnClickListener mOnColorClickListener = new View.OnClickListener() {
		public void onClick(View v) {
			if (v.getClass().equals(ColorView.class)) {
				final ColorView cv = (ColorView) v;
				mProvider.setColor(cv.getColor());
			}
		}
	};

	private final View.OnTouchListener mOnTouchListener = new View.OnTouchListener() {
		public boolean onTouch(View v, MotionEvent event) {
			if (event.getAction() != MotionEvent.ACTION_MOVE) {
				return true;
			}
			if (v.equals(mColorPicker)) {
				if (!mChangeIsInProgress.getAndSet(true)) {
					final int x0 = (int) event.getX() - mRadius;
					final int y0 = (int) event.getY() - mRadius;
					final double magnitude = Math.hypot(x0, y0);
					final double alpha = _180_DIVIDED_PI
							* Math.acos(x0 / magnitude);
					final float hue = (float) ((y0 < 0) ? alpha : 360 - alpha);
					final float saturation = (float) Math.min(1d, magnitude
							/ mRadius);
					mProvider.setColorHueSaturation(hue, saturation);
					mChangeIsInProgress.set(false);
				}
			} else if (v.equals(mValuePicker)) {
				if (!mChangeIsInProgress.getAndSet(true)) {
					final float value = Math.max(0f,
							Math.min(1f, event.getX() / v.getWidth()));
					mProvider.setColorValue(value);
					mChangeIsInProgress.set(false);
				}
			} else {
				return false;
			}
			return true;
		}
	};

	@Override
	public int getIcon() {
		return R.drawable.ic_action_location_searching;
	}

	public void onColorChanged(float[] hsv, int argb) {
		if (null != mColorStatus) {
			mColorStatus.setColor(Color.HSVToColor(hsv));
			setCrosshairs(hsv);
			setValueGradient(hsv);
		}
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup group,
			Bundle savedInstanceState) {
		View v = inflater.inflate(R.layout.fragment_set_color, group, false);
		v.getViewTreeObserver().addOnGlobalLayoutListener(this);
		mColorStatus = (ColorView) v.findViewById(R.id.colorStatus);
		mCrosshair = (ImageView) v.findViewById(R.id.crosshair);
		mColorPicker = (ImageView) v.findViewById(R.id.colorPicker);
		mValuePicker = (View) v.findViewById(R.id.valuePicker);
		mValueCrosshair = (ImageView) v.findViewById(R.id.valueCrosshair);

		// connect all color buttons with the OnClickListener
		for (int id : COLOR_BUTTONS) {
			ColorView colorBtn = (ColorView) v.findViewById(id);
			colorBtn.setOnClickListener(mOnColorClickListener);
		}
		return v;
	}

	@Override
	public void onDetach() {
		super.onDetach();
	}

	public void onGlobalLayout() {
		if (this.isDetached()) {
			mProvider.removeOnColorChangedListener(this);
			return;
		}
		mDiameter = mColorPicker.getWidth();
		mRadius = mDiameter / 2;
		mRadiusCrosshair = mCrosshair.getWidth() / 2;
		final int shift = (mColorPicker.getWidth() - mCrosshair.getWidth()) / 2;
		mCenterX = mColorPicker.getX() + shift;
		mCenterY = mColorPicker.getY() + shift;
		mCenterZ = mValuePicker.getX() - mRadiusCrosshair;
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
		mCrosshair.setX(mCenterX + (float) x0);
		mCrosshair.setY(mCenterY + (float) y0);
		mValueCrosshair.setX(mCenterZ + value);
	}

	/**
	 * updates the hsv value pickers background gradient
	 * @param hsv
	 *            should be an array of three floats: hsv[0] is Hue [0 .. 360)
	 *            hsv[1] is Saturation [0...1] hsv[2] is Value [0...1]
	 */
	private void setValueGradient(float[] ref) {
		final float[] hsv = new float[] { ref[0], ref[1], 1f };
		ShapeDrawable background = new ShapeDrawable(new RectShape());
		LinearGradient gradient = new LinearGradient(0, 0,
				mValuePicker.getWidth(), 0, Color.BLACK, Color.HSVToColor(hsv),
				Shader.TileMode.CLAMP);
		background.getPaint().setShader(gradient);
		mValuePicker.setBackgroundDrawable(background);
	}
}
