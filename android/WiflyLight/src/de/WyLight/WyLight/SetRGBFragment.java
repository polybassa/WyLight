package de.WyLight.WyLight;

import java.util.concurrent.atomic.AtomicBoolean;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewTreeObserver;
import de.WyLight.WyLight.R;
import de.WyLight.WyLight.view.RgbGradientView;

public class SetRGBFragment extends ControlFragment implements
		ViewTreeObserver.OnGlobalLayoutListener {
	AtomicBoolean mChangeIsInProgress = new AtomicBoolean(false);
	private RgbGradientView mRGB = null;

	@Override
	public int getIcon() {
		return R.drawable.ic_action_sort_by_size;
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup group,
			Bundle savedInstanceState) {

		View view = inflater.inflate(R.layout.fragment_set_rgb, group, false);
		view.getViewTreeObserver().addOnGlobalLayoutListener(this);

		mRGB = (RgbGradientView) view.findViewById(R.id.rgb_volume);
		mRGB.setOnColorChangedListener(new RgbGradientView.OnColorChangeListener() {
			public void onColorChanged(int[] argb) {

				if (!mChangeIsInProgress.getAndSet(true)) {
					mProvider.setColor(argb[0]);
					mChangeIsInProgress.set(false);
				}
			}
		});
		return view;
	}

	public void onGlobalLayout() {
		if (isDetached()) {
			mProvider.removeOnColorChangedListener(mRGB);
		} else {
			mProvider.addOnColorChangedListener(mRGB);
		}
	}
}
