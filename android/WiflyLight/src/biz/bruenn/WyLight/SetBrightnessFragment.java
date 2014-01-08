package biz.bruenn.WyLight;

import java.util.concurrent.atomic.AtomicBoolean;

import biz.bruenn.WiflyLight.R;
import biz.bruenn.WyLight.view.VolumeView;

import android.graphics.Color;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewTreeObserver;

public class SetBrightnessFragment extends ControlFragment implements OnColorChangeListener, ViewTreeObserver.OnGlobalLayoutListener {

	private VolumeView mVolume = null;

	@Override
	public int getIcon() {
		return R.drawable.ic_action_brightness_high;
	}

	public void onColorChanged(int color) {
		if(null != mVolume) {
			final float[] hsv = new float[3];
			Color.colorToHSV(color, hsv);
			mVolume.setVolume((int) (hsv[2]*100));
		}
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup group, Bundle savedInstanceState) {

		View view = inflater.inflate(R.layout.fragment_set_brightness, group, false);	

		mVolume = (VolumeView)view.findViewById(R.id.brightnessPicker);
		mProvider.addOnColorChangedListener(this);
		mVolume.setOnVolumeChangedListener(new VolumeView.OnVolumeChangedListener() {
			private AtomicBoolean mChangeIsInProgress = new AtomicBoolean(false);
			
			public void onVolumeChanged(int percent) {
				if(!mChangeIsInProgress.getAndSet(true)) {		
					final int intensity = (int)(2.55f * percent);
					final int c = (((intensity << 8) | intensity) << 8) | intensity;
					setColor(c);
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
