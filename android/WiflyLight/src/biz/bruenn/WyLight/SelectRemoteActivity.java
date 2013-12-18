package biz.bruenn.WyLight;

import java.util.ArrayList;

import biz.bruenn.WiflyLight.R;
import biz.bruenn.WiflyLight.R.id;
import biz.bruenn.WiflyLight.R.string;
import biz.bruenn.WyLight.library.Endpoint;
import biz.bruenn.WyLight.library.EndpointAdapter;
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
import android.widget.Button;
import android.widget.ListView;
import android.widget.ProgressBar;
import android.widget.TextView;

public class SelectRemoteActivity extends Activity implements RemoteCollector.OnPostExecuteListener {
	private ArrayList<Endpoint> mRemoteArray = new ArrayList<Endpoint>();
	private EndpointAdapter mRemoteArrayAdapter;
	private ListView mRemoteList;
	private BroadcastReceiver mBroadcastReceiver;
	private Button mScanBtn;
	private ProgressBar mProgress;

	static {
		System.loadLibrary("wifly");
	}

	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_select_remote);

		mProgress = (ProgressBar)findViewById(R.id.progress);

		mBroadcastReceiver = new BroadcastReceiver(this.getFilesDir().getAbsolutePath() + "/recent.txt");
		mRemoteList = (ListView)findViewById(id.remoteList);
		registerForContextMenu(mRemoteList);
		mRemoteArrayAdapter = new EndpointAdapter(this, android.R.layout.simple_list_item_1, mRemoteArray);
		mRemoteList.setAdapter(mRemoteArrayAdapter);
		mRemoteList.setEmptyView((TextView)findViewById(android.R.id.empty));
		mRemoteList.setOnItemClickListener(new AdapterView.OnItemClickListener() {

			public void onItemClick(AdapterView<?> arg0, View v, int arg2,
					long arg3) {
				Endpoint e = mRemoteArrayAdapter.getItem(arg2);
				Intent i = new Intent(v.getContext(), WiflyControlActivity.class);
				i.putExtra(WiflyControlActivity.EXTRA_ENDPOINT, e);
				startActivityForResult(i, 0);
			}
		});

		mScanBtn = (Button)findViewById(id.scan);

		mScanBtn.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
				Button btn = (Button)v;
				btn.setClickable(false);
				btn.setText(string.scanning);
				mProgress.setVisibility(View.VISIBLE);
				new RemoteCollector(mBroadcastReceiver, (WifiManager)getSystemService(Context.WIFI_SERVICE),
						mRemoteArray,
						mRemoteArrayAdapter,
						SelectRemoteActivity.this).execute(Long.valueOf(3000000000L));
			}
		});
		// add all recent endpoints to our list view
		for(long index = 0;;index++) {
			Endpoint e = mBroadcastReceiver.getEndpoint(index);
			if(!e.isValid()) {
				break;
			}
			mRemoteArray.add(e);
		}
		mRemoteArrayAdapter.notifyDataSetChanged();
	}

	@Override
	public void onCreateContextMenu(ContextMenu menu, View v, ContextMenuInfo menuInfo) {
		super.onCreateContextMenu(menu, v, menuInfo);

		AdapterContextMenuInfo info = (AdapterContextMenuInfo)menuInfo;
		showDialog(info.position);
	}

	@Override
	protected Dialog onCreateDialog(int position, Bundle savedInstanceState) {
		if(0 <= position && position < mRemoteArrayAdapter.getCount()) {
			return new SetWlanDialog(this, mRemoteArrayAdapter.getItem(position));
		} else {
			// this is to omit a bug when switching from SoftAP to client mode
			return null;
		}
	}

	@Override
	protected void onDestroy() {
		mBroadcastReceiver.release();
		mBroadcastReceiver = null;
		super.onDestroy();
	}

	public void onPostExecute() {
		mScanBtn.setClickable(true);
		mScanBtn.setText(string.scan);
		mProgress.setVisibility(View.GONE);
	}
}
