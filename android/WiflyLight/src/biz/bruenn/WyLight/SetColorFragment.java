package biz.bruenn.WyLight;

import java.util.concurrent.atomic.AtomicBoolean;

import biz.bruenn.WiflyLight.R;

import android.graphics.Bitmap;
import android.graphics.Color;
import android.graphics.drawable.BitmapDrawable;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;

public class SetColorFragment extends ControlFragment {
	private static final double PI_DIVIDED_MINUS_180 = Math.PI / -180;
	private Button mColorStatus;
	private ImageView mCrosshair;
	private TextView mDebug;
	private TextView mDebugAngle;
	private Button mColorDebug;
	private ImageView mColorPicker;

	@Override
	public int getIcon() {
		return R.drawable.ic_action_location_searching;
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup group, Bundle savedInstanceState) {
		View v = inflater.inflate(R.layout.fragment_set_color, group, false);
		mColorStatus = (Button)v.findViewById(R.id.colorStatus);
		mColorDebug = (Button)v.findViewById(R.id.colorDebug);
		mCrosshair = (ImageView)v.findViewById(R.id.crosshair);
		mDebug = (TextView)v.findViewById(R.id.debug);
		mDebugAngle = (TextView)v.findViewById(R.id.debugAngle);
		mColorPicker = (ImageView)v.findViewById(R.id.colorPicker);
		mColorPicker.setOnTouchListener(new View.OnTouchListener() {
			private AtomicBoolean mChangeIsInProgress = new AtomicBoolean(false);
			
			public boolean onTouch(View v, MotionEvent event) {
				if(!mChangeIsInProgress.getAndSet(true)) {
					final Bitmap bmp = ((BitmapDrawable)((ImageView)v).getDrawable()).getBitmap();
					final double diameter = bmp.getWidth();
					final double radius = diameter/2d;
					final double x0 = Math.max(0, Math.min(diameter-1, event.getX())) - radius;
					final double y0 = Math.max(0, Math.min(diameter-1, event.getY())) - radius;
					final double magnitude = Math.hypot(x0, y0);
					final double alpha = 180*Math.acos(x0/magnitude)/Math.PI;
					final double hue = (y0 < 0) ? alpha : 360 - alpha;
					final double saturation = Math.min(1d, magnitude/radius);
					final double value = 1d;
					final float[] hsv = new float[]{(float) hue, (float) saturation, (float) value};
					final int color = Color.HSVToColor(hsv);
					onSetColor(color);
					mColorStatus.setBackgroundColor(color);
					setCrosshair(color, bmp.getWidth());
					mDebug.setText(Integer.toHexString(color));
					mDebugAngle.setText(String.valueOf(saturation));

					mChangeIsInProgress.set(false);
				}
				return true;
			}
		});
		return v;
	}

	private void setCrosshair(int color, int width) {
		float[] hsv = new float[3];
		Color.colorToHSV(color, hsv);
		final double hue = hsv[0] * PI_DIVIDED_MINUS_180;
		final double saturation = hsv[1] * width / 2;

		final double shift = (width - mCrosshair.getWidth()) / 2d;
		final double centerX = mColorPicker.getX() + shift;
		final double centerY = mColorPicker.getY() + shift;

		double x0 = Math.cos(hue) * saturation;
		double y0 = Math.sin(hue) * saturation;

		mCrosshair.setX((float) (centerX + x0));
		mCrosshair.setY((float) (centerY + y0));
	}
}
