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
				boolean done = mCtrl.fwSetColor(mColor, WiflyControl.ALL_LEDS);
				Toast.makeText(v.getContext(), String.valueOf(done), Toast.LENGTH_SHORT).show();
			}
		});
		
		ImageView colorPicker = (ImageView)v.findViewById(R.id.colorPicker);
		colorPicker.setOnTouchListener(new View.OnTouchListener() {	
			public boolean onTouch(View v, MotionEvent event) {
				Bitmap b = ((BitmapDrawable)((ImageView)v).getDrawable()).getBitmap();
				int x = Math.max(0, Math.min(b.getWidth()-1, (int)event.getX()));
				int y = Math.max(0, Math.min(b.getHeight()-1, (int)event.getY()));
				mColor = b.getPixel(x, y);
				mSetColorBtn.setBackgroundColor(mColor);
				//TODO remove debug code
				//mSetColorBtn.setText(Integer.toHexString(mColor));
				mSetColorBtn.setTextColor(0xff000000 | ~mColor);
				return true;
			}
		});
		return v;
	}
}
