package biz.bruenn.WyLight.library;

import java.io.File;

import android.content.Context;
import android.graphics.Color;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;

public class ScriptManagerAdapter extends BaseAdapter {
	
	private native String getScriptName(String path, int index);
	private native int numScripts(String path);
	private native void newScript(String path, String name);
	private native void saveScript(String path, long pNative);
	private final String mPath; 
	
	public ScriptManagerAdapter(Context context) {
		mPath = context.getFilesDir().getAbsolutePath();
	}

	public void add(String scriptName) {
		newScript(mPath+'/'+scriptName+".wyscript", scriptName);
		this.notifyDataSetChanged();
	}

	public int getCount() {
		return numScripts(mPath);
	}

	public ScriptAdapter getItem(int position) {
		return new ScriptAdapter(mPath + '/' + getScriptName(mPath, position));
	}

	public long getItemId(int position) {
		// TODO Auto-generated method stub
		return 0;
	}

	public View getView(int position, View convertView, ViewGroup parent) {
		// TODO Auto-generated method stub
		TextView x = new TextView(parent.getContext());
		x.setTextColor(Color.BLACK);
		x.setText(getScriptName(mPath, position));
		return x;
	}
	
	public void remove(ScriptAdapter script) {
		new File(mPath + '/' + script.getName()).delete();
		notifyDataSetChanged();
	}
	
	public void save(ScriptAdapter script) {
		if(null != script && !script.mDesignatedForDeletion) {
			final String fullPath = mPath+'/'+script.getName();
			saveScript(fullPath, script.getNative());
		}
	}
}
