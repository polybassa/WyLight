package biz.bruenn.WiflyLight;

import java.util.ArrayList;

import biz.bruenn.WiflyLight.R.id;
import biz.bruenn.WiflyLight.R.string;
import android.app.Activity;
import android.content.Context;
import android.net.wifi.WifiManager;
import android.os.Bundle;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;

public class WiflyLightActivity extends Activity {
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

        mRemoteList = (ListView)findViewById(id.remoteList);
        mRemoteArrayAdapter = new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1, mRemoteArray);
        mRemoteList.setAdapter(mRemoteArrayAdapter);
        
        Button scanBtn = (Button)findViewById(id.scan);
        scanBtn.setOnClickListener(new View.OnClickListener() {
			
			public void onClick(View v) {
				Button btn = (Button)v;
				btn.setClickable(false);
				btn.setText(string.scanning);
				new RemoteCollector((WifiManager)getSystemService(Context.WIFI_SERVICE), 
						mRemoteArray,
						mRemoteArrayAdapter,
						btn).execute(Long.valueOf(3000));
			}
		});
    }
}