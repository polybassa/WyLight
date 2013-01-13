package biz.bruenn.WiflyLight;

import java.util.ArrayList;

import biz.bruenn.WiflyLight.R.id;
import biz.bruenn.WiflyLight.R.string;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.net.wifi.WifiManager;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.Toast;

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
        mRemoteList.setOnItemClickListener(new AdapterView.OnItemClickListener() {

			public void onItemClick(AdapterView<?> arg0, View v, int arg2,
					long arg3) {
				mRemoteArrayAdapter.getItem(arg2);
				Intent i = new Intent(v.getContext(), WiflyControlActivity.class);
				i.putExtra("IpAddress", 0x0A000202);
				i.putExtra("Port", (short)2000);
				startActivityForResult(i, 0);
				/*
				WiflyControl ctrl = new WiflyControl(0x0A000202, (short)2000);
				boolean done = ctrl.fwSetColor(0xffffffff, 0xff000000);
				Toast.makeText(getApplicationContext(), String.valueOf(done), Toast.LENGTH_SHORT).show();
				*/
			}
		});
        
        Button scanBtn = (Button)findViewById(id.scan);
        scanBtn.setOnClickListener(new View.OnClickListener() {
			
			public void onClick(View v) {
				Button btn = (Button)v;
				btn.setClickable(false);
				btn.setText(string.scanning);
				new RemoteCollector((WifiManager)getSystemService(Context.WIFI_SERVICE), 
						mRemoteArray,
						mRemoteArrayAdapter,
						btn).execute(Long.valueOf(3000000000L));
			}
		});
    }
}