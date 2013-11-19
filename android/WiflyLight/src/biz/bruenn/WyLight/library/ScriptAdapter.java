package biz.bruenn.WyLight.library;

import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;

public class ScriptAdapter extends BaseAdapter {
	
	private native void addFade(long pNative, int argb, int addr, short fadeTime);
	private native void clear(long pNative);
	private native long getItem(long pNative, int position);
	private native int numCommands(long pNative);
	private final long mNative;
	
	ScriptAdapter(long pNative) {
		mNative = pNative;
	}
	
	public void addFade(int argb, int addr, short fadeTime) {
		addFade(mNative, argb, addr, fadeTime);
		notifyDataSetChanged();
	}
	
	public void clear() {
		clear(mNative);
		notifyDataSetChanged();
	}

	public int getCount() {
		return numCommands(mNative);
	}

	public FwCmdScriptAdapter getItem(int position) {
		// TODO Auto-generated method stub
		return new FwCmdScriptAdapter(getItem(mNative, position));
	}

	public long getItemId(int position) {
		// TODO Auto-generated method stub
		return 0;
	}

	public View getView(int position, View convertView, ViewGroup parent) {
		TextView v = new TextView(parent.getContext());
		v.setBackgroundColor(getItem(position).getColor());
		return v;
	}
}
