package biz.bruenn.WiflyLight;

import java.util.concurrent.atomic.AtomicBoolean;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.SeekBar;
import android.widget.SeekBar.OnSeekBarChangeListener;

public class SetBrightnessFragment extends ControlFragment {

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup group, Bundle savedInstanceState) {

		View view = inflater.inflate(R.layout.fragment_set_brightness, group, false);	

		SeekBar brightness = (SeekBar)view.findViewById(R.id.brightnessPicker);
		brightness.setOnSeekBarChangeListener(new OnSeekBarChangeListener() {
			private AtomicBoolean mChangeIsInProgress = new AtomicBoolean(false);

			public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
				if(!mChangeIsInProgress.getAndSet(true)) {		
					final int intensity = (int)(2.55f * progress);
					final int c = (((intensity << 8) | intensity) << 8) | intensity;
					mCtrl.fwSetColor(c, WiflyControl.ALL_LEDS);
					mChangeIsInProgress.set(false);
				}
			}
			
			public void onStartTrackingTouch(SeekBar seekBar) {
				//do nothing				
			}
			
			public void onStopTrackingTouch(SeekBar seekBar) {
				//do nothing
			}
		});
		return view;
	}
}
