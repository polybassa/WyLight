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
					setCrosshair(color, bmp);
					mDebug.setText(Integer.toHexString(color));
					mDebugAngle.setText(String.valueOf(saturation));

					mChangeIsInProgress.set(false);
				}
				return true;
			}
		});
		return v;
	}

	private void setCrosshair(int color, Bitmap bmp) {
		float[] hsv = new float[3];
		Color.colorToHSV(color, hsv);
		final double hue = hsv[0] * Math.PI / -180d;
		final double saturation = hsv[1] * (bmp.getWidth() - 0) / 2;

		final double shiftCrosshair =  mCrosshair.getWidth() / 2d;
		final double shift = bmp.getWidth() / 2;

		double x0 = Math.cos(hue) * saturation + shift;
		double y0 = Math.sin(hue) * saturation + shift;

		if(x0 < 0) throw new RuntimeException("x0: " + String.valueOf(x0));
		if(y0 < 0) throw new RuntimeException("y0: " + String.valueOf(y0));
		mCrosshair.setX((float) (mColorPicker.getX() + x0 - shiftCrosshair));
		mCrosshair.setY((float) (mColorPicker.getY() + y0 - shiftCrosshair));

		//mDebugAngle.setText(String.valueOf(hsv[0]));
		mDebugAngle.setText(String.valueOf(x0));
		int x = Math.max(0, Math.min(bmp.getWidth()-1, (int)x0));
		int y = Math.max(0, Math.min(bmp.getWidth()-1, (int)y0));
		int debugColor = bmp.getPixel(x, y);
		mColorDebug.setBackgroundColor(debugColor);
	}
}
