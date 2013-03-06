package biz.bruenn.WiflyLight;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.SeekBar;
import android.widget.SeekBar.OnSeekBarChangeListener;
import android.widget.Toast;

public class SetBrightnessFragment extends ControlFragment {

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup group, Bundle savedInstanceState) {

		View view = inflater.inflate(R.layout.fragment_set_brightness, group, false);	

		SeekBar brightness = (SeekBar)view.findViewById(R.id.brightnessPicker);
		brightness.setOnSeekBarChangeListener(new OnSeekBarChangeListener() {
			public void onProgressChanged(SeekBar seekBar, int progress,
					boolean fromUser) {
				// TODO beatify
				int intensity = (int)(2.55f * progress);
				int c = ((((intensity << 8) | intensity) << 8) | intensity) << 8;
				boolean done = mCtrl.fwSetColor(WiflyControl.ALL_LEDS, c);
				Toast.makeText(seekBar.getContext(), String.valueOf(done), Toast.LENGTH_SHORT).show();	
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
