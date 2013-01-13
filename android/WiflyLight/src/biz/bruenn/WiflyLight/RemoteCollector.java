package biz.bruenn.WiflyLight;

import java.util.ArrayList;
import biz.bruenn.WiflyLight.R.string;

import android.net.wifi.WifiManager;
import android.os.AsyncTask;
import android.widget.ArrayAdapter;
import android.widget.Button;

public class RemoteCollector extends AsyncTask<Long, Void, Void> {
	private native long createBroadcastReceiver();
	private native long getNextRemote(long pNative, long timeoutNanos);
	private native void releaseBroadcastReceiver(long pNative);
	
	private ArrayAdapter<String> mRemoteArrayAdapter;
	private ArrayList<String> mRemoteArray;
	private WifiManager.MulticastLock mMulticastLock; 
	private long mNative = 0;
	private Button mScanBtn;
	
	public RemoteCollector(WifiManager wifiMgr, ArrayList<String> remoteArray, ArrayAdapter<String> remoteArrayAdapter, Button scanBtn) {
		mMulticastLock = wifiMgr.createMulticastLock("WiflyLight_MulticastLock");
		mNative = createBroadcastReceiver();
		mRemoteArray = remoteArray;
		mRemoteArrayAdapter = remoteArrayAdapter;
		mScanBtn = scanBtn;
	}
	
	public void finalize() throws Throwable {
		releaseBroadcastReceiver(mNative);
	}

	public void run() {
	}

	@Override
	protected Void doInBackground(Long... params) {
		long timeoutNanos = params[0];
		final long endTime = System.nanoTime() + timeoutNanos;
		
		mMulticastLock.acquire();
		while(!isCancelled() && (timeoutNanos > 0)) {
			long remote = getNextRemote(mNative, timeoutNanos);
			if(0 != remote)
			{
				mRemoteArray.add(String.valueOf(remote));
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
