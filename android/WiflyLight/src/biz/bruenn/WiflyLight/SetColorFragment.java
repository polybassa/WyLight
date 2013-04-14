package biz.bruenn.WiflyLight;

import java.util.concurrent.atomic.AtomicBoolean;

import biz.bruenn.WiflyLight.exception.ConnectionTimeout;

import android.graphics.Bitmap;
import android.graphics.drawable.BitmapDrawable;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.Toast;

public class SetColorFragment extends ControlFragment {

	private int mColor = 0xff000000;
	private Button mSetColorBtn;

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup group, Bundle savedInstanceState) {
		View v = inflater.inflate(R.layout.fragment_set_color, group, false);

		mSetColorBtn = (Button)v.findViewById(R.id.setColor);
		mSetColorBtn.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
				try {
					mCtrl.fwSetColor(mColor, WiflyControl.ALL_LEDS);
				} catch (ConnectionTimeout e) {
					Toast.makeText(v.getContext(), "Connection lost", Toast.LENGTH_SHORT).show();
					// TODO Auto-generated catch block
					e.printStackTrace();
					getActivity().finish();
				}
			}
		});
		
		ImageView colorPicker = (ImageView)v.findViewById(R.id.colorPicker);
		colorPicker.setOnTouchListener(new View.OnTouchListener() {
			private AtomicBoolean mChangeIsInProgress = new AtomicBoolean(false);
			
			public boolean onTouch(View v, MotionEvent event) {
				Bitmap b = ((BitmapDrawable)((ImageView)v).getDrawable()).getBitmap();
				int x = Math.max(0, Math.min(b.getWidth()-1, (int)event.getX()));
				int y = Math.max(0, Math.min(b.getHeight()-1, (int)event.getY()));
				mColor = b.getPixel(x, y);
				mSetColorBtn.setBackgroundColor(mColor);
				mSetColorBtn.setTextColor(0xff000000 | ~mColor);
				if(!mChangeIsInProgress.getAndSet(true)) {
					try {
						mCtrl.fwSetColor(mColor, WiflyControl.ALL_LEDS);
					} catch (ConnectionTimeout e) {
						Toast.makeText(v.getContext(), "Connection lost", Toast.LENGTH_SHORT).show();
						// TODO Auto-generated catch block
						e.printStackTrace();
						getActivity().finish();
					} finally {
						mChangeIsInProgress.set(false);
					}
				}
				return true;
			}
		});
		return v;
	}
}
