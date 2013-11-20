package biz.bruenn.WyLight.library;

import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;

public class ScriptAdapter extends BaseAdapter {
	
	private native void addFade(long pNative, int argb, int addr, short fadeTime);
	private native void clear(long pNative);
	private native long create(String filename);
	private native long getItem(long pNative, int position);
	private native String name(long pNative);
	private native int numCommands(long pNative);
	private final long mNative;
	public boolean mDesignatedForDeletion = false;
	
	ScriptAdapter(long pNative) {
		mNative = pNative;
	}
	
	ScriptAdapter(String filename) {
		mNative = create(filename);
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
	
	public String getName() {
		return name(mNative);
	}
	
	public long getNative() {
		return mNative;
	}

	public View getView(int position, View convertView, ViewGroup parent) {
		TextView v = new TextView(parent.getContext());
		v.setBackgroundColor(getItem(position).getColor());
		return v;
	}
}
