package biz.bruenn.WyLight;

import java.util.ArrayList;

import android.net.wifi.WifiManager;
import android.os.AsyncTask;
import android.widget.ArrayAdapter;

public class RemoteCollector extends AsyncTask<Long, Void, Void> {
	
	public interface OnPostExecuteListener {
		public void onPostExecute();
	}

	private ArrayAdapter<Endpoint> mRemoteArrayAdapter;
	private ArrayList<Endpoint> mRemoteArray;
	private WifiManager.MulticastLock mMulticastLock;
	private BroadcastReceiver mBroadcastReceiver;
	private OnPostExecuteListener mListener;
	
	public RemoteCollector(BroadcastReceiver receiver, WifiManager wifiMgr, ArrayList<Endpoint> remoteArray, ArrayAdapter<Endpoint> remoteArrayAdapter, OnPostExecuteListener listener) {
		mMulticastLock = wifiMgr.createMulticastLock("WiflyLight_MulticastLock");
		mBroadcastReceiver = receiver;
		mRemoteArray = remoteArray;
		mRemoteArrayAdapter = remoteArrayAdapter;
		mListener = listener;
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
				final int index = mRemoteArray.indexOf(remote);
				if(-1 == index) {
					remote.setOnline(true);
					mRemoteArray.add(remote);
				} else {
					mRemoteArray.get(index).setOnline(true);
				}
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
		if(null != mListener) {
			mListener.onPostExecute();
		}
	}
}
