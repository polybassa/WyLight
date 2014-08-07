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

import de.WyLight.WyLight.R;
import de.WyLight.WyLight.view.ColorView;

import android.graphics.Color;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewTreeObserver;
import android.widget.SeekBar;

public class SetBrightnessFragment extends ControlFragment implements
		OnColorChangeListener, ViewTreeObserver.OnGlobalLayoutListener {

	private SeekBar mVolume = null;
	private ColorView mColorStatus;

	@Override
	public int getIcon() {
		return R.drawable.ic_action_brightness_high;
	}

	public void onColorChanged(float[] color, int argb) {
		mColorStatus.setColor(Color.HSVToColor(color));
		mVolume.setProgress((int) (color[2] * 100));
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup group,
			Bundle savedInstanceState) {

		View view = inflater.inflate(R.layout.fragment_set_brightness, group,
				false);

		mColorStatus = (ColorView) view.findViewById(R.id.colorStatus);
		mVolume = (SeekBar) view.findViewById(R.id.brightnessPicker);
		mProvider.addOnColorChangedListener(this);
		mVolume.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {

			private AtomicBoolean mChangeIsInProgress = new AtomicBoolean(false);

			public void onStopTrackingTouch(SeekBar seekBar) {
				/* not implemented */
			}

			public void onStartTrackingTouch(SeekBar seekBar) {
				/* not implemented */
			}

			public void onProgressChanged(SeekBar seekBar, int progress,
					boolean fromUser) {
				if (fromUser && !mChangeIsInProgress.getAndSet(true)) {
					mProvider.setColorValue(0.01f * progress);
					mChangeIsInProgress.set(false);
				}
			}
		});
		return view;
	}

	public void onGlobalLayout() {
		if (isDetached()) {
			mProvider.removeOnColorChangedListener(this);
		} else {
			mProvider.addOnColorChangedListener(this);
		}
	}
}
