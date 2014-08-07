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
import de.WyLight.WyLight.library.ScriptAdapter;
import android.app.Activity;
import android.app.Fragment;
import android.view.Menu;

public abstract class ControlFragment extends Fragment {

	protected WiflyControlProvider mProvider;

	public interface WiflyControlProvider {
		public void addOnColorChangedListener(OnColorChangeListener listener);

		public void removeOnColorChangedListener(OnColorChangeListener listener);

		public void sendScript(ScriptAdapter script);

		public void setColor(int argb);

		public void setColorHueSaturation(float hue, float saturation);

		public void setColorValue(float value);
	}

	/**
	 * This logo can be used for example as a navigation button in the ActionBar
	 * @return a positive integer used to identify the id of the fragments logo
	 */
	public abstract int getIcon();

	@Override
	public void onAttach(Activity activity) {
		super.onAttach(activity);
		try {
			mProvider = (WiflyControlProvider) activity;
		} catch (ClassCastException e) {
			throw new ClassCastException(activity.toString()
					+ " must implement WiflyControlProvider!");
		}
	}

	/**
	 * Should be called when the fragment is shown, to update the menu
	 * @param menu
	 */
	public void onShow(Menu menu) {
		menu.findItem(R.id.action_add).setVisible(false);
		menu.findItem(R.id.action_stop).setVisible(true);
	}
}