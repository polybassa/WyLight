package biz.bruenn.WyLight;

import biz.bruenn.WiflyLight.R;
import biz.bruenn.WyLight.exception.ConnectionTimeout;
import biz.bruenn.WyLight.exception.FatalError;
import biz.bruenn.WyLight.exception.ScriptBufferFull;
import android.app.Activity;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentActivity;
import android.widget.Toast;

public class ControlFragment extends Fragment {

	protected WiflyControlProvider mProvider;
	
	public interface WiflyControlProvider {
		public WiflyControl getControl();
	}
	
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
		final FragmentActivity activity = getActivity();
		Toast.makeText(activity, "Connection lost", Toast.LENGTH_SHORT).show();
		activity.finish();		
	}
	
	protected void onSetColor(int color) {
		try {
			mProvider.getControl().fwSetColor(color, WiflyControl.ALL_LEDS);
		} catch (ConnectionTimeout e) {
			onConnectionLost();
		} catch (ScriptBufferFull e) {
			Toast.makeText(getActivity(), R.string.msg_scriptbufferfull, Toast.LENGTH_LONG).show();
		} catch (FatalError e) {
			Toast.makeText(getActivity(), e.getMessage(), Toast.LENGTH_SHORT).show();
		}		
	}
	
	protected void onSetFade(int color, short time) {	
		try {
			mProvider.getControl().fwSetFade(color, WiflyControl.ALL_LEDS, time);
		} catch (ConnectionTimeout e) {
			onConnectionLost();
		} catch (ScriptBufferFull e) {
			Toast.makeText(getActivity(), R.string.msg_scriptbufferfull, Toast.LENGTH_LONG).show();		
		} catch (FatalError e) {
			Toast.makeText(getActivity(), e.getMessage(), Toast.LENGTH_SHORT).show();
		}		
	}
	
	protected void onSetGradient(int color_1, int color_2, int length, int offset, short time) {	
		try {
			mProvider.getControl().fwSetGradient(color_1, color_2, length, offset, time);
		} catch (ConnectionTimeout e) {
			onConnectionLost();
		} catch (ScriptBufferFull e) {
			Toast.makeText(getActivity(), R.string.msg_scriptbufferfull, Toast.LENGTH_LONG).show();		
		} catch (FatalError e) {
			Toast.makeText(getActivity(), e.getMessage(), Toast.LENGTH_SHORT).show();
		}		
	}
}