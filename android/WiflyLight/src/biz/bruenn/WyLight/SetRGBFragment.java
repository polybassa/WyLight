package biz.bruenn.WyLight;

import java.util.concurrent.atomic.AtomicBoolean;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import biz.bruenn.WiflyLight.R;
import biz.bruenn.WyLight.view.RgbVolumeView;

public class SetRGBFragment extends ControlFragment {	
	AtomicBoolean mChangeIsInProgress = new AtomicBoolean(false);

	@Override
	public int getIcon() {
		return R.drawable.ic_action_sort_by_size;
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup group, Bundle savedInstanceState) {

		View view = inflater.inflate(R.layout.fragment_set_rgb, group, false);	

		RgbVolumeView rgb = (RgbVolumeView)view.findViewById(R.id.rgb_volume);
		rgb.setOnColorChangedListener(new RgbVolumeView.OnColorChangedListener() {
			public void onColorChanged(int color) {

				if(!mChangeIsInProgress.getAndSet(true)) {
					onSetColor(color);
					mChangeIsInProgress.set(false);
				}
			}
		});
		return view;
	}
}
