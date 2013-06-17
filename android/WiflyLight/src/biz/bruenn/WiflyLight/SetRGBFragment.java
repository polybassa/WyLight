package biz.bruenn.WiflyLight;

import java.util.concurrent.atomic.AtomicBoolean;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import biz.bruenn.WiflyLight.view.VolumeView;

public class SetRGBFragment extends ControlFragment {
	int mRed = 0;
	int mGreen = 0;
	int mBlue = 0;
	
	AtomicBoolean mChangeIsInProgress = new AtomicBoolean(false);

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup group, Bundle savedInstanceState) {

		View view = inflater.inflate(R.layout.fragment_set_rgb, group, false);	

		VolumeView red = (VolumeView)view.findViewById(R.id.redVolume);
		red.setOnVolumeChangedListener(new VolumeView.OnVolumeChangedListener() {			
			public void onVolumeChanged(int percent) {
				final int intensity = (int)(2.55f * percent);
				mRed = 0xff000000 | ((0x000000ff & intensity) << 16);
				if(!mChangeIsInProgress.getAndSet(true)) {
					onSetColor(mRed | mGreen | mBlue);
					mChangeIsInProgress.set(false);
				}
			}
		});	

		VolumeView green = (VolumeView)view.findViewById(R.id.greenVolume);
		green.setOnVolumeChangedListener(new VolumeView.OnVolumeChangedListener() {			
			public void onVolumeChanged(int percent) {
				final int intensity = (int)(2.55f * percent);
				mGreen = ((0x000000ff & intensity) << 8);
				if(!mChangeIsInProgress.getAndSet(true)) {
					onSetColor(mRed | mGreen | mBlue);
					mChangeIsInProgress.set(false);
				}
			}
		});	

		VolumeView blue = (VolumeView)view.findViewById(R.id.blueVolume);
		blue.setOnVolumeChangedListener(new VolumeView.OnVolumeChangedListener() {			
			public void onVolumeChanged(int percent) {
				final int intensity = (int)(2.55f * percent);
				mBlue = 0x000000ff & intensity;
				if(!mChangeIsInProgress.getAndSet(true)) {
					onSetColor(mRed | mGreen | mBlue);
					mChangeIsInProgress.set(false);
				}
			}
		});
		return view;
	}

}
