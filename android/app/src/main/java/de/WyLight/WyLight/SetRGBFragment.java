/**
                Copyright (C) 2012 - 2014 Patrick Bruenn.

    This file is part of WyLight.

    Wylight is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    WiflyLight is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with WiflyLight.  If not, see <http://www.gnu.org/licenses/>. */

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
