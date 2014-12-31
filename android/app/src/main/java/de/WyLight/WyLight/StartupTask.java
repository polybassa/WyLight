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

import de.WyLight.WyLight.R;
import de.WyLight.WyLight.exception.FatalError;
import de.WyLight.WyLight.library.Endpoint;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.os.AsyncTask;

public class StartupTask extends AsyncTask<WiflyControl, String, Void>
		implements WiflyControl.StartupListener {

	private final Activity mParent;
	private final String mPath;
	private final Endpoint mRemote;
	private final AlertDialog mStartupDialog;

	public StartupTask(Activity parent, Endpoint remote, String path) {
		mParent = parent;
		mPath = path;
		mRemote = remote;
		AlertDialog.Builder builder = new AlertDialog.Builder(parent);
		builder.setTitle(R.string.title_dialog_startup);
		builder.setMessage(R.string.msg_connecting);
		mStartupDialog = builder.create();
		mStartupDialog
				.setOnCancelListener(new DialogInterface.OnCancelListener() {
					public void onCancel(DialogInterface dialog) {
						StartupTask.this.cancel(true);
						mParent.finish();
					}
				});
		mStartupDialog.show();
	}

	@Override
	protected Void doInBackground(WiflyControl... params) {
		WiflyControl ctrl = params[0];
		try {
			ctrl.startup(mRemote, mPath, this);
		} catch (FatalError e) {
			mStartupDialog.cancel();
		}
		mStartupDialog.dismiss();
		return null;
	}

	@Override
	protected void onProgressUpdate(String... progress) {
		mStartupDialog.setMessage(progress[0]);
	}

	public void setMessage(String message) {
		publishProgress(message);
		/*
		 * Enable to delay startup messages try { Thread.sleep(1000, 0); } catch
		 * (InterruptedException e) { // TODO Auto-generated catch block
		 * e.printStackTrace(); } //
		 */
	}
}
