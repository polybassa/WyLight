package biz.bruenn.WyLight;

import java.util.concurrent.atomic.AtomicBoolean;

import biz.bruenn.WiflyLight.R;
import biz.bruenn.WyLight.view.VolumeView;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

public class SetBrightnessFragment extends ControlFragment {

	@Override
	public int getIcon() {
		return R.drawable.ic_action_brightness_high;
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup group, Bundle savedInstanceState) {

		View view = inflater.inflate(R.layout.fragment_set_brightness, group, false);	

		VolumeView volume = (VolumeView)view.findViewById(R.id.brightnessPicker);
		volume.setOnVolumeChangedListener(new VolumeView.OnVolumeChangedListener() {
			private AtomicBoolean mChangeIsInProgress = new AtomicBoolean(false);
			
			public void onVolumeChanged(int percent) {
				if(!mChangeIsInProgress.getAndSet(true)) {		
					final int intensity = (int)(2.55f * percent);
					final int c = (((intensity << 8) | intensity) << 8) | intensity;
					onSetColor(c);
					mChangeIsInProgress.set(false);
				}
			}
		});
		return view;
	}
}
