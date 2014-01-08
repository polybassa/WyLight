package biz.bruenn.WyLight;

import biz.bruenn.WiflyLight.R;
import biz.bruenn.WyLight.exception.ConnectionTimeout;
import biz.bruenn.WyLight.exception.FatalError;
import biz.bruenn.WyLight.exception.ScriptBufferFull;
import biz.bruenn.WyLight.library.ScriptAdapter;
import android.app.Activity;
import android.app.Fragment;
import android.widget.Toast;

public abstract class ControlFragment extends Fragment {

	protected WiflyControlProvider mProvider;
	
	public interface WiflyControlProvider {
		public void addOnColorChangedListener(OnColorChangeListener listener);
		public WiflyControl getControl();
		public void removeOnColorChangedListener(OnColorChangeListener listener);
		public void setColor(int color);
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
	
	protected void onConnectionLost() {
		final Activity activity = getActivity();
		Toast.makeText(activity, "Connection lost", Toast.LENGTH_SHORT).show();
		activity.finish();		
	}
	
	protected void onFatalError(FatalError e) {
		Toast.makeText(getActivity(), e.getMessage(), Toast.LENGTH_SHORT).show();		
	}
	
	protected void onScriptBufferFull() {
		Toast.makeText(getActivity(), R.string.msg_scriptbufferfull, Toast.LENGTH_LONG).show();		
	}
	
	protected void onSendScript(ScriptAdapter script) {
		try {
			mProvider.getControl().fwClearScript();
			mProvider.getControl().fwSendScript(script);
		} catch (ConnectionTimeout e) {
			onConnectionLost();
		} catch (ScriptBufferFull e) {
			onScriptBufferFull();
		} catch (FatalError e) {
			onFatalError(e);
		}
	}
	
	protected void setColor(int color) {
		try {
			mProvider.setColor(color);
			mProvider.getControl().fwSetColor(color, WiflyControl.ALL_LEDS);
		} catch (ConnectionTimeout e) {
			onConnectionLost();
		} catch (ScriptBufferFull e) {
			onScriptBufferFull();
		} catch (FatalError e) {
			onFatalError(e);
		}		
	}
	
	protected void onSetFade(int color, short time) {	
		try {
			mProvider.getControl().fwSetFade(color, WiflyControl.ALL_LEDS, time);
		} catch (ConnectionTimeout e) {
			onConnectionLost();
		} catch (ScriptBufferFull e) {
			onScriptBufferFull();		
		} catch (FatalError e) {
			onFatalError(e);
		}		
	}
	
	protected void onSetGradient(int color_1, int color_2, int length, int offset, short time) {	
		try {
			mProvider.getControl().fwSetGradient(color_1, color_2, length, offset, time);
		} catch (ConnectionTimeout e) {
			onConnectionLost();
		} catch (ScriptBufferFull e) {
			onScriptBufferFull();		
		} catch (FatalError e) {
			onFatalError(e);
		}		
	}
}