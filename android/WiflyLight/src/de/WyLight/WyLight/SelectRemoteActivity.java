/**
                Copyright (C) 2012 - 2014 Patrick Bruenn.

    This file is part of WyLight.

    Wylight is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    WiflyLight is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with WiflyLight.  If not, see <http://www.gnu.org/licenses/>. */

package de.WyLight.WyLight;

import java.util.ArrayList;

import de.WyLight.WyLight.R;
import de.WyLight.WyLight.R.id;
import de.WyLight.WyLight.R.string;
import de.WyLight.WyLight.library.Endpoint;
import de.WyLight.WyLight.library.EndpointAdapter;
import android.app.Activity;
import android.app.FragmentManager;
import android.content.Context;
import android.content.Intent;
import android.net.wifi.WifiManager;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.ListView;
import android.widget.ProgressBar;
import android.widget.TextView;

public class SelectRemoteActivity extends Activity implements
		RemoteCollector.OnPostExecuteListener {
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

		mProgress = (ProgressBar) findViewById(R.id.progress);

		mBroadcastReceiver = new BroadcastReceiver(this.getFilesDir()
				.getAbsolutePath() + "/recent.txt");
		mRemoteList = (ListView) findViewById(id.remoteList);
		registerForContextMenu(mRemoteList);
		mRemoteArrayAdapter = new EndpointAdapter(this,
				android.R.layout.simple_list_item_1, mRemoteArray);
		mRemoteList.setAdapter(mRemoteArrayAdapter);
		mRemoteList.setEmptyView((TextView) findViewById(android.R.id.empty));
		mRemoteList.setOnItemClickListener(new AdapterView.OnItemClickListener() {
					public void onItemClick(AdapterView<?> arg0, View v, int arg2, long arg3) {
						Endpoint e = mRemoteArrayAdapter.getItem(arg2);
						Intent i = new Intent(v.getContext(), WiflyControlActivity.class);
						i.putExtra(WiflyControlActivity.EXTRA_ENDPOINT, e);
						startActivityForResult(i, 0);
					}
				});
		mRemoteList.setOnItemLongClickListener(new AdapterView.OnItemLongClickListener() {
					public boolean onItemLongClick(AdapterView<?> arg0,	View arg1, int position, long arg3) {
						FragmentManager fm = getFragmentManager();
						SetWlanDialog d = new SetWlanDialog();
						d.setRemote(mRemoteArrayAdapter.getItem(position));
						d.show(fm, "HUHU");
						return true;
					}
				});

		mScanBtn = (Button) findViewById(id.scan);

		mScanBtn.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
				scan();
			}
		});
		// add all recent endpoints to our list view
		for (long index = 0;; index++) {
			Endpoint e = mBroadcastReceiver.getEndpoint(index);
			if (!e.isValid()) {
				break;
			}
			mRemoteArray.add(e);
		}
		mRemoteArrayAdapter.notifyDataSetChanged();
	}

	@Override
	protected void onDestroy() {
		mBroadcastReceiver.release();
		mBroadcastReceiver = null;
		super.onDestroy();
	}

	@Override
	protected void onResume() {
		super.onResume();
		scan();
	}

	public void onPostExecute() {
		mScanBtn.setClickable(true);
		mScanBtn.setText(string.action_scan);
		mProgress.setVisibility(View.GONE);
	}

	private void scan() {
		mScanBtn.setClickable(false);
		mScanBtn.setText(string.scanning);
		mProgress.setVisibility(View.VISIBLE);
		new RemoteCollector(mBroadcastReceiver,
				(WifiManager) getSystemService(Context.WIFI_SERVICE),
				mRemoteArray, mRemoteArrayAdapter,
				SelectRemoteActivity.this).execute(Long.valueOf(3000000000L));
	}
}
