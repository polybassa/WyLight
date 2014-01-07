package biz.bruenn.WyLight;

import java.util.concurrent.atomic.AtomicBoolean;

import biz.bruenn.WiflyLight.R;

import android.graphics.Bitmap;
import android.graphics.drawable.BitmapDrawable;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ImageView;

public class SetColorFragment extends ControlFragment {

	private Button mColorStatus;
	private ImageView mCrosshair;

	@Override
	public int getIcon() {
		return R.drawable.ic_action_location_searching;
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup group, Bundle savedInstanceState) {
		View v = inflater.inflate(R.layout.fragment_set_color, group, false);
		
		mColorStatus = (Button)v.findViewById(R.id.colorStatus);
		mCrosshair = (ImageView)v.findViewById(R.id.crosshair);
		ImageView colorPicker = (ImageView)v.findViewById(R.id.colorPicker);
		colorPicker.setOnTouchListener(new View.OnTouchListener() {
			private AtomicBoolean mChangeIsInProgress = new AtomicBoolean(false);
			
			public boolean onTouch(View v, MotionEvent event) {
				final Bitmap bmp = ((BitmapDrawable)((ImageView)v).getDrawable()).getBitmap();
				int x = Math.max(0, Math.min(bmp.getWidth()-1, (int)event.getX()));
				int y = Math.max(0, Math.min(bmp.getHeight()-1, (int)event.getY()));
				if(!mChangeIsInProgress.getAndSet(true)) {
					double x0 = x - bmp.getWidth()/2;
					double y0 = y - bmp.getHeight()/2;
					final double factor = (0.95d * bmp.getWidth()) / (2d * Math.sqrt(x0*x0+y0*y0));
					if(factor < 1d) {
						x0 *= factor;
						y0 *= factor;
						x = (int) Math.round(x0 + bmp.getWidth()/2);
						y = (int) Math.round(y0 + bmp.getWidth()/2);
					}
					mCrosshair.setX(v.getX() + x - mCrosshair.getWidth()/2);
					mCrosshair.setY(v.getY() + y - mCrosshair.getHeight()/2);
					final int color = bmp.getPixel(x, y);
					onSetColor(color);
					mColorStatus.setBackgroundColor(color);
					mChangeIsInProgress.set(false);
				}
				return true;
			}
		});
		return v;
	}
}
