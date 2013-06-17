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
import android.widget.ImageView;

public class SetColorFunFragment extends ControlFragment {

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup group, Bundle savedInstanceState) {
		View v = inflater.inflate(R.layout.fragment_set_color_fun, group, false);
		
		ImageView colorPicker = (ImageView)v.findViewById(R.id.colorPicker);
		colorPicker.setOnTouchListener(new View.OnTouchListener() {
			private AtomicBoolean mChangeIsInProgress = new AtomicBoolean(false);
			
			public boolean onTouch(View v, MotionEvent event) {
				final Bitmap bmp = ((BitmapDrawable)((ImageView)v).getDrawable()).getBitmap();
				final int x = Math.max(0, Math.min(bmp.getWidth()-1, (int)event.getX()));
				final int y = Math.max(0, Math.min(bmp.getHeight()-1, (int)event.getY()));
				if(!mChangeIsInProgress.getAndSet(true)) {
					onSetColor(bmp.getPixel(x, y));
					mChangeIsInProgress.set(false);
				}
				return true;
			}
		});
		return v;
	}
}
