package biz.bruenn.WyLight.view;

import biz.bruenn.WiflyLight.R;
import biz.bruenn.WyLight.OnColorChangeListener;
import android.content.Context;
import android.graphics.Color;
import android.graphics.drawable.GradientDrawable;
import android.util.AttributeSet;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.widget.LinearLayout;
import android.widget.SeekBar;

public class RgbGradientView extends LinearLayout implements OnColorChangeListener {

	public interface OnColorChangeListener {
		void onColorChanged(int[] argb);
	}

	private ColorView mColorStatus;
	private ColorView mRedStatus;
	private ColorView mGreenStatus;
	private ColorView mBlueStatus;
	private OnColorChangeListener mOnColorChangedListener = null;
	private SeekBar mRed;
	private SeekBar mGreen;
	private SeekBar mBlue;
	private int mActive = 0;
	private int[] mColors = new int[]{Color.WHITE};

	private SeekBar.OnSeekBarChangeListener mListener = new SeekBar.OnSeekBarChangeListener() {

		public void onStopTrackingTouch(SeekBar seekBar) {
			/* not implemented */
		}

		public void onStartTrackingTouch(SeekBar seekBar) {
			/* not implemented */
		}

		public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
			final int color = getColor();
			mColors[mActive] = color;
			if(1 == mColors.length) {
				mColorStatus.setGradient(new GradientDrawable(GradientDrawable.Orientation.LEFT_RIGHT, new int[] {mColors[0], mColors[0]}));
			} else {
				mColorStatus.setGradient(new GradientDrawable(GradientDrawable.Orientation.LEFT_RIGHT, mColors));
			}
			mRedStatus.setColor(0xffff0000 & color);
			mRedStatus.setText(String.valueOf(Color.red(color) * 100 / 255) + '%');
			mGreenStatus.setColor(0xff00ff00 & color);
			mGreenStatus.setText(String.valueOf(Color.green(color) * 100 / 255) + '%');
			mBlueStatus.setColor(0xff0000ff & color);
			mBlueStatus.setText(String.valueOf(Color.blue(color) * 100 / 255) + '%');
			if(fromUser && (null != mOnColorChangedListener)) {
				mOnColorChangedListener.onColorChanged(mColors);
			}
		}
	};

	public RgbGradientView(Context context, AttributeSet attrib) {
		super(context, attrib);
		LayoutInflater i = (LayoutInflater)context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
		i.inflate(R.layout.view_rgb_volume, this, true);

		mColorStatus = (ColorView)this.findViewById(R.id.colorStatus);
		enableStatusTouchable();
		mRedStatus = (ColorView)this.findViewById(R.id.redStatus);
		mGreenStatus = (ColorView)this.findViewById(R.id.greenStatus);
		mBlueStatus = (ColorView)this.findViewById(R.id.blueStatus);

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

	public void onColorChanged(float[] hsv, int argb) {
		mRed.setProgress(Color.red(argb));
		mGreen.setProgress(Color.green(argb));
		mBlue.setProgress(Color.blue(argb));
	}

	public void setColors(int[] colors) {
		mColors = colors;
	}

	private void enableStatusTouchable() {
		mColorStatus.setClickable(true);
		mColorStatus.setOnTouchListener(new View.OnTouchListener() {
			public boolean onTouch(View v, MotionEvent event) {
				if(event.getAction() == MotionEvent.ACTION_UP) {
					final float sectorSize = v.getWidth() / mColors.length;
					final int raw = (int)(event.getX() / sectorSize);
					mActive = Math.max(0, Math.min(raw, mColors.length - 1));
					onColorChanged(null, mColors[mActive]);
					return true;
				}
				return false;
			}
		});
	}

	public void setOnColorChangedListener(OnColorChangeListener listener) {
		mOnColorChangedListener = listener;
	}
}
