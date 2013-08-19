package biz.bruenn.WyLight.view;

import biz.bruenn.WiflyLight.R;
import android.content.Context;
import android.util.AttributeSet;
import android.view.LayoutInflater;
import android.widget.LinearLayout;

public class RgbVolumeView extends LinearLayout {
	
	public interface OnColorChangedListener {
		public void onColorChanged(int color);
	}

	private OnColorChangedListener mOnColorChangedListener = null;
	private int mRed = 0;
	private int mGreen = 0;
	private int mBlue = 0;

	public RgbVolumeView(Context context, AttributeSet attrib) {
		super(context, attrib);
		LayoutInflater i = (LayoutInflater)context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
		i.inflate(R.layout.view_rgb_volume, this, true);

		VolumeView red = (VolumeView)this.findViewById(R.id.redVolume);
		red.setOnVolumeChangedListener(new VolumeView.OnVolumeChangedListener() {			
			public void onVolumeChanged(int percent) {
				final int intensity = (int)(2.55f * percent);
				mRed = 0xff000000 | ((0x000000ff & intensity) << 16);
				updateColor();
			}
		});	

		VolumeView green = (VolumeView)this.findViewById(R.id.greenVolume);
		green.setOnVolumeChangedListener(new VolumeView.OnVolumeChangedListener() {			
			public void onVolumeChanged(int percent) {
				final int intensity = (int)(2.55f * percent);
				mGreen = ((0x000000ff & intensity) << 8);
				updateColor();
			}
		});	

		VolumeView blue = (VolumeView)this.findViewById(R.id.blueVolume);
		blue.setOnVolumeChangedListener(new VolumeView.OnVolumeChangedListener() {			
			public void onVolumeChanged(int percent) {
				final int intensity = (int)(2.55f * percent);
				mBlue = 0x000000ff & intensity;
				updateColor();
			}
		});
	}
	
	public void setOnColorChangedListener(OnColorChangedListener listener) {
		mOnColorChangedListener = listener;
	}
	
	private void updateColor() {
		if(null != mOnColorChangedListener) {
			mOnColorChangedListener.onColorChanged(mRed | mGreen | mBlue);
		}
	}

}
