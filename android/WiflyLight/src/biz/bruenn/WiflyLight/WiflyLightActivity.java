package biz.bruenn.WiflyLight;

import java.util.ArrayList;

import biz.bruenn.WiflyLight.R.id;
import android.app.Activity;
import android.content.Context;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.wifi.WifiManager;
import android.os.Bundle;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.TextView;

public class WiflyLightActivity extends Activity {
	
	private RemoteCollector mRemoteCollector;
	private ListView mRemoteList;
	private ArrayList<String> mRemoteArray = new ArrayList<String>();
	ArrayAdapter<String> mRemoteArrayAdapter;
	
	static {
		System.loadLibrary("wifly");
	}
	
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

        mRemoteArray.add("1");
        mRemoteArray.add("2");
        mRemoteList = (ListView)findViewById(id.remoteList);
        mRemoteArrayAdapter = new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1, mRemoteArray);
        mRemoteCollector = new RemoteCollector(mRemoteArrayAdapter);
        mRemoteList.setAdapter(mRemoteArrayAdapter);
       
        
        Button scanBtn = (Button)findViewById(id.scan);
        scanBtn.setOnClickListener(new View.OnClickListener() {
			
			public void onClick(View v) {
				WifiManager wifiMgr = (WifiManager)getSystemService(Context.WIFI_SERVICE);
				if(wifiMgr != null) {
					WifiManager.MulticastLock lock = wifiMgr.createMulticastLock("WiflyLight_MulticastLock");
					lock.acquire();

					ConnectivityManager connMgr = (ConnectivityManager)getSystemService(Context.CONNECTIVITY_SERVICE);
					NetworkInfo info = connMgr.getActiveNetworkInfo();
					TextView debug = (TextView)findViewById(R.id.debugOutput);
					if(null != info && info.isConnected()) {
						mRemoteCollector.run();
					} else {
						debug.setText("no network connection found");
					}
					lock.release();
				}
			}
		});
    }
}