package biz.bruenn.WiflyLight;

import android.graphics.Bitmap;
import android.graphics.drawable.BitmapDrawable;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ImageView;

public class SetFadeFragment extends ControlFragment {

	private int mColor = 0xff000000;
	private Button mSetFadeBtn;

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup group, Bundle savedInstanceState) {
		View v = inflater.inflate(R.layout.fragment_set_fade, group, false);
		
		mSetFadeBtn = (Button)v.findViewById(R.id.setFade);
		mSetFadeBtn.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
				onSetFade(mColor, (short)500);
			}
		});
		
		ImageView colorPicker = (ImageView)v.findViewById(R.id.colorPicker);
		colorPicker.setOnTouchListener(new View.OnTouchListener() {
			public boolean onTouch(View v, MotionEvent event) {
				Bitmap b = ((BitmapDrawable)((ImageView)v).getDrawable()).getBitmap();
				int x = Math.max(0, Math.min(b.getWidth()-1, (int)event.getX()));
				int y = Math.max(0, Math.min(b.getHeight()-1, (int)event.getY()));
				mColor = b.getPixel(x, y);
				mSetFadeBtn.setBackgroundColor(mColor);
				mSetFadeBtn.setTextColor(0xff000000 | ~mColor);
				return true;
			}
		});
		return v;
	}
}
