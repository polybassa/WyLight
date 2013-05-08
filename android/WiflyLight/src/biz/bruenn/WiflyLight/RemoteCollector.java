package biz.bruenn.WiflyLight;

import java.util.ArrayList;
import biz.bruenn.WiflyLight.R.string;

import android.net.wifi.WifiManager;
import android.os.AsyncTask;
import android.widget.ArrayAdapter;
import android.widget.Button;

public class RemoteCollector extends AsyncTask<Long, Void, Void> {
	
	private ArrayAdapter<Endpoint> mRemoteArrayAdapter;
	private ArrayList<Endpoint> mRemoteArray;
	private WifiManager.MulticastLock mMulticastLock;
	private BroadcastReceiver mBroadcastReceiver;
	private Button mScanBtn;
	
	public RemoteCollector(BroadcastReceiver receiver, WifiManager wifiMgr, ArrayList<Endpoint> remoteArray, ArrayAdapter<Endpoint> remoteArrayAdapter, Button scanBtn) {
		mMulticastLock = wifiMgr.createMulticastLock("WiflyLight_MulticastLock");
		mBroadcastReceiver = receiver;
		mRemoteArray = remoteArray;
		mRemoteArrayAdapter = remoteArrayAdapter;
		mScanBtn = scanBtn;
	}

	public void run() {
	}

	@Override
	protected Void doInBackground(Long... params) {
		long timeoutNanos = params[0];
		final long endTime = System.nanoTime() + timeoutNanos;
		
		mMulticastLock.acquire();
		while(!isCancelled() && (timeoutNanos > 0)) {
			Endpoint remote = mBroadcastReceiver.getNextRemote(timeoutNanos);
			
			if(remote.isValid())
			{
				mRemoteArray.add(remote);
				publishProgress();
			}
			timeoutNanos = endTime - System.nanoTime();
		}
		mMulticastLock.release();
		return null;
	}

	@Override
	protected void onProgressUpdate(Void... progress) {
		mRemoteArrayAdapter.notifyDataSetChanged();
	}
	
	@Override
	protected void onPostExecute(Void result) {
		mScanBtn.setClickable(true);
		mScanBtn.setText(string.scan);
	}
}
