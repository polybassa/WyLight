package biz.bruenn.WyLight;

import biz.bruenn.WiflyLight.R;
import biz.bruenn.WyLight.exception.ConnectionTimeout;
import biz.bruenn.WyLight.exception.FatalError;
import biz.bruenn.WyLight.exception.ScriptBufferFull;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentActivity;
import android.widget.Toast;

public class ControlFragment extends Fragment {

	protected WiflyControl mCtrl;
	
	protected void onConnectionLost() {
		final FragmentActivity activity = getActivity();
		Toast.makeText(activity, "Connection lost", Toast.LENGTH_SHORT).show();
		activity.finish();		
	}
	
	protected void onSetColor(int color) {
		try {
			mCtrl.fwSetColor(color, WiflyControl.ALL_LEDS);
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
			mCtrl.fwSetFade(color, WiflyControl.ALL_LEDS, time);
		} catch (ConnectionTimeout e) {
			onConnectionLost();
		} catch (ScriptBufferFull e) {
			Toast.makeText(getActivity(), R.string.msg_scriptbufferfull, Toast.LENGTH_LONG).show();		
		} catch (FatalError e) {
			Toast.makeText(getActivity(), e.getMessage(), Toast.LENGTH_SHORT).show();
		}		
	}

	public void setWiflyControl(WiflyControl ctrl) {
		mCtrl = ctrl;
	}
}