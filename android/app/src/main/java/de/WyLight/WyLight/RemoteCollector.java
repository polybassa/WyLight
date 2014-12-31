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

import de.WyLight.WyLight.library.Endpoint;

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

	public RemoteCollector(BroadcastReceiver receiver, WifiManager wifiMgr,
			ArrayList<Endpoint> remoteArray,
			ArrayAdapter<Endpoint> remoteArrayAdapter,
			OnPostExecuteListener listener) {
		mMulticastLock = wifiMgr
				.createMulticastLock("WiflyLight_MulticastLock");
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
		while (!isCancelled() && (timeoutNanos > 0)) {
			Endpoint remote = mBroadcastReceiver.getNextRemote(timeoutNanos);

			if (remote.isValid()) {
				final int index = mRemoteArray.indexOf(remote);
				if (-1 == index) {
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
		if (null != mListener) {
			mListener.onPostExecute();
		}
	}
}
