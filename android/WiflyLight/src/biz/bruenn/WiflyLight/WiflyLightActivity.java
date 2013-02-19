package biz.bruenn.WiflyLight;

import java.util.ArrayList;

import biz.bruenn.WiflyLight.R.id;
import biz.bruenn.WiflyLight.R.string;
import android.app.Activity;
import android.app.Dialog;
import android.content.Context;
import android.content.Intent;
import android.net.wifi.WifiManager;
import android.os.Bundle;
import android.view.ContextMenu;
import android.view.ContextMenu.ContextMenuInfo;
import android.view.View;
import android.widget.AdapterView;
import android.widget.AdapterView.AdapterContextMenuInfo;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;

public class WiflyLightActivity extends Activity {
	private ListView mRemoteList;
	private ArrayList<Endpoint> mRemoteArray = new ArrayList<Endpoint>();
	private ArrayAdapter<Endpoint> mRemoteArrayAdapter;
	
	static {
		System.loadLibrary("wifly");
	}
	
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

        mRemoteList = (ListView)findViewById(id.remoteList);
        registerForContextMenu(mRemoteList);
        mRemoteArrayAdapter = new ArrayAdapter<Endpoint>(this, android.R.layout.simple_list_item_1, mRemoteArray);
        mRemoteList.setAdapter(mRemoteArrayAdapter);
        mRemoteList.setOnItemClickListener(new AdapterView.OnItemClickListener() {

			public void onItemClick(AdapterView<?> arg0, View v, int arg2,
					long arg3) {
				Endpoint e = mRemoteArrayAdapter.getItem(arg2);
				Intent i = new Intent(v.getContext(), WiflyControlActivity.class);
				i.putExtra(WiflyControlActivity.EXTRA_IP, e.getAddr());
				i.putExtra(WiflyControlActivity.EXTRA_PORT, e.getPort());
				startActivityForResult(i, 0);    	
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
    
    @Override
    public void onCreateContextMenu(ContextMenu menu, View v, ContextMenuInfo menuInfo) {
    	super.onCreateContextMenu(menu, v, menuInfo);

    	AdapterContextMenuInfo info = (AdapterContextMenuInfo)menuInfo;
		showDialog(info.position);
    }
    
    @Override
    protected Dialog onCreateDialog(int position, Bundle savedInstanceState) {
    	return new StringPickerDialog(this, mRemoteArrayAdapter.getItem(position));
    }
}
