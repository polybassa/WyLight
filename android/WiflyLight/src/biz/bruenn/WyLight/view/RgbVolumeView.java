package biz.bruenn.WyLight.view;

import biz.bruenn.WiflyLight.R;
import biz.bruenn.WyLight.OnColorChangeListener;
import android.content.Context;
import android.graphics.Color;
import android.util.AttributeSet;
import android.view.LayoutInflater;
import android.widget.LinearLayout;
import android.widget.SeekBar;

public class RgbVolumeView extends LinearLayout implements OnColorChangeListener {

	private ColorView mColorStatus;
	private OnColorChangeListener mOnColorChangedListener = null;
	private SeekBar mRed;
	private SeekBar mGreen;
	private SeekBar mBlue;

	private SeekBar.OnSeekBarChangeListener mListener = new SeekBar.OnSeekBarChangeListener() {

		public void onStopTrackingTouch(SeekBar seekBar) {
			/* not implemented */
		}

		public void onStartTrackingTouch(SeekBar seekBar) {
			/* not implemented */
		}

		public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
			final int color = getColor();
			mColorStatus.setColor(color);
			if(fromUser && (null != mOnColorChangedListener)) {
				mOnColorChangedListener.onColorChanged(color);
			}
		}
	};

	public RgbVolumeView(Context context, AttributeSet attrib) {
		super(context, attrib);
		LayoutInflater i = (LayoutInflater)context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
		i.inflate(R.layout.view_rgb_volume, this, true);

		mColorStatus = (ColorView)this.findViewById(R.id.colorStatus);

		mRed = (SeekBar)this.findViewById(R.id.redVolume);
		mRed.setOnSeekBarChangeListener(mListener);

		mGreen = (SeekBar)this.findViewById(R.id.greenVolume);
		mGreen.setOnSeekBarChangeListener(mListener);

		mBlue = (SeekBar)this.findViewById(R.id.blueVolume);
		mBlue.setOnSeekBarChangeListener(mListener);
	}
	
	private int getColor() {
		return Color.rgb(mRed.getProgress(), mGreen.getProgress(), mBlue.getProgress());
	}

	public void onColorChanged(int color) {
		mRed.setProgress(Color.red(color));
		mGreen.setProgress(Color.green(color));
		mBlue.setProgress(Color.blue(color));
	}

	public void setOnColorChangedListener(OnColorChangeListener listener) {
		mOnColorChangedListener = listener;
	}
}
