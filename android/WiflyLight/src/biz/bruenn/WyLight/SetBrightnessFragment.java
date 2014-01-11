package biz.bruenn.WyLight;

import java.util.concurrent.atomic.AtomicBoolean;

import biz.bruenn.WiflyLight.R;
import biz.bruenn.WyLight.view.ColorView;

import android.graphics.Color;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewTreeObserver;
import android.widget.SeekBar;

public class SetBrightnessFragment extends ControlFragment implements OnColorChangeListener, ViewTreeObserver.OnGlobalLayoutListener {

	private SeekBar mVolume = null;
	private final float[] mHSV = new float[3];
	private ColorView mColorStatus;

	@Override
	public int getIcon() {
		return R.drawable.ic_action_brightness_high;
	}

	public void onColorChanged(int color) {
		mColorStatus.setColor(color);
		Color.colorToHSV(color, mHSV);
		mVolume.setProgress((int) (mHSV[2]*100));
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup group, Bundle savedInstanceState) {

		View view = inflater.inflate(R.layout.fragment_set_brightness, group, false);	

		mColorStatus = (ColorView)view.findViewById(R.id.colorStatus);
		mVolume = (SeekBar)view.findViewById(R.id.brightnessPicker);
		mProvider.addOnColorChangedListener(this);
		mVolume.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
			
			private AtomicBoolean mChangeIsInProgress = new AtomicBoolean(false);

			public void onStopTrackingTouch(SeekBar seekBar) {
				/* not implemented */
			}

			public void onStartTrackingTouch(SeekBar seekBar) {
				/* not implemented */
			}

			public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
				if(fromUser && !mChangeIsInProgress.getAndSet(true)) {
					mHSV[2] = 0.01f*progress;
					setColor(Color.HSVToColor(mHSV));
					mChangeIsInProgress.set(false);
				}
			}
		});
		return view;
	}

	public void onGlobalLayout() {
		if(isDetached()) {
			mProvider.removeOnColorChangedListener(this);
		} else {
			mProvider.addOnColorChangedListener(this);
		}
	}
}
