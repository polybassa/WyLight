package biz.bruenn.WyLight;

import biz.bruenn.WyLight.R;
import biz.bruenn.WyLight.library.ScriptAdapter;
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
			mProvider = (WiflyControlProvider)activity;
		} catch (ClassCastException e) {
			throw new ClassCastException(activity.toString() + " must implement WiflyControlProvider!");
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