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

import de.WyLight.WyLight.R;
import de.WyLight.WyLight.view.RgbGradientView;
import android.app.Activity;
import android.graphics.Color;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.widget.SeekBar;
import android.widget.TextView;

public class EditGradientActivity extends Activity {

	private int[] mColor;
	private TextView mTimeStatus;
	private SeekBar mTime;

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_edit_gradient);

		mColor = getIntent().getIntArrayExtra(EditScriptActivity.ITEM_COLORS);
		if (null == mColor) {
			mColor = new int[] { Color.WHITE, Color.BLACK };
		}
		RgbGradientView rgb = (RgbGradientView) this
				.findViewById(R.id.rgb_volume);
		rgb.setColors(mColor);
		rgb.onColorChanged(null, mColor[0]);
		rgb.setOnColorChangedListener(new RgbGradientView.OnColorChangeListener() {
			public void onColorChanged(int[] color) {
				mColor = color;
			}
		});
		mTimeStatus = (TextView) findViewById(R.id.timeStatus);
		mTime = (SeekBar) findViewById(R.id.timeVolume);
		mTime.setMax(FadeTime.getMaxIndex());
		mTime.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
			public void onStopTrackingTouch(SeekBar seekBar) {
				// TODO Auto-generated method stub
			}

			public void onStartTrackingTouch(SeekBar seekBar) {
				// TODO Auto-generated method stub
			}

			public void onProgressChanged(SeekBar seekBar, int progress,
					boolean fromUser) {
				updateTimeStatus(progress);
			}
		});
		final int time = getIntent().getIntExtra(EditScriptActivity.ITEM_TIME,
				0);
		mTime.setProgress(FadeTime.timeToIndex(time));
		updateTimeStatus(mTime.getProgress());
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		MenuInflater inflater = getMenuInflater();
		inflater.inflate(R.menu.edit_gradient_activity_actions, menu);
		return super.onCreateOptionsMenu(menu);
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		switch (item.getItemId()) {
		case R.id.action_accept:
			saveAndFinish();
			return true;
		default:
			return super.onOptionsItemSelected(item);
		}
	}

	private void saveAndFinish() {
		final int fadeTmms = FadeTime.indexToTmms(mTime.getProgress());
		getIntent().putExtra(EditScriptActivity.ITEM_TIME, fadeTmms);
		getIntent().putExtra(EditScriptActivity.ITEM_COLORS, mColor);
		setResult(mColor[0], getIntent());
		finish();
	}

	private void updateTimeStatus(int duration) {
		mTimeStatus.setText("Duration: " + Integer.toString(duration) + " sec");
	}
}
