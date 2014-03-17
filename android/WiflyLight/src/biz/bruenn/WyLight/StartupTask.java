package biz.bruenn.WyLight;

import biz.bruenn.WyLight.R;
import biz.bruenn.WyLight.exception.FatalError;
import biz.bruenn.WyLight.library.Endpoint;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.os.AsyncTask;

public class StartupTask extends AsyncTask<WiflyControl, String, Void> implements WiflyControl.StartupListener {

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
		mStartupDialog.setOnCancelListener(new DialogInterface.OnCancelListener() {
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
		/* Enable to delay startup messages
		try {
			Thread.sleep(1000, 0);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		//*/
	}
}
