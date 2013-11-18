package biz.bruenn.WyLight.library;

import android.content.Context;
import android.graphics.Color;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;

public class ScriptManagerAdapter extends BaseAdapter {
	
	private native long getScript(String path, long index);
	private native String getScriptName(String path, long index);
	private native int numScripts(String path);
	private native void newScript(String path, String name);
	private final String path; 
	
	public ScriptManagerAdapter(Context context) {
		path = context.getFilesDir().getAbsolutePath();
	}

	public void add(String scriptName) {
		// TODO send only path and scriptname to library
		newScript(path+'/'+scriptName+".wyscript", scriptName);
	}

	public int getCount() {
		return numScripts(path);
	}

	public ScriptAdapter getItem(int position) {
		// TODO Auto-generated method stub
		return new ScriptAdapter(getScript(path, position));
	}

	public long getItemId(int position) {
		// TODO Auto-generated method stub
		return 0;
	}

	public View getView(int position, View convertView, ViewGroup parent) {
		// TODO Auto-generated method stub
		TextView x = new TextView(parent.getContext());
		x.setTextColor(Color.BLACK);
		x.setText(getScriptName(path, (long)position));
		return x;
	}
}
