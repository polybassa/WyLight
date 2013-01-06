package biz.bruenn.WiflyLight;

import android.app.Activity;
import android.content.Context;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.wifi.WifiManager;
import android.os.Bundle;
import android.widget.TextView;

public class WiflyLightActivity extends Activity {
	private native long createBroadcastReceiver();
	private native long getNumRemotes(long pNative);
	private native void releaseBroadcastReceiver(long pNative);
	
	static {
		System.loadLibrary("wifly");
	}
	
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
   
        WifiManager wifiMgr = (WifiManager)getSystemService(Context.WIFI_SERVICE);
        if(wifiMgr != null) {
        	WifiManager.MulticastLock lock = wifiMgr.createMulticastLock("WiflyLight_MulticastLock");
        	lock.acquire();

	        ConnectivityManager connMgr = (ConnectivityManager)getSystemService(Context.CONNECTIVITY_SERVICE);
	        NetworkInfo info = connMgr.getActiveNetworkInfo();
	        TextView debug = (TextView)findViewById(R.id.debugOutput);
	        if(null != info && info.isConnected()) {
	        	long pNative = createBroadcastReceiver();
	        	debug.setText("b " + String.valueOf(getNumRemotes(pNative)));
	        	releaseBroadcastReceiver(pNative);
	        } else {
	        	debug.setText("no network connection found");
	        }
	        lock.release();
        }
    }
}