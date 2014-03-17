package biz.bruenn.WyLight.library;

import java.io.File;

import android.content.Context;
import android.graphics.Color;
import android.graphics.drawable.GradientDrawable;
import android.util.DisplayMetrics;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowManager;
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
		notifyDataSetChanged();
	}

	public int getCount() {
		return numScripts(mPath);
	}

	public ScriptAdapter getItem(int position) {
		return new ScriptAdapter(mPath + '/' + getScriptName(mPath, position));
	}

	public long getItemId(int position) {
		return getItem(position).getNative();
	}

	public View getView(int position, View convertView, ViewGroup parent) {
		WindowManager wm = (WindowManager)parent.getContext().getSystemService(Context.WINDOW_SERVICE);
		DisplayMetrics metrics = new DisplayMetrics();
		wm.getDefaultDisplay().getMetrics(metrics);

		TextView x = new TextView(parent.getContext());
		x.setHeight((int)(0.5f * metrics.xdpi));
		x.setWidth(metrics.widthPixels);
		int[] tmp = getItem(position).getColors();
		int[] colors;
		if(tmp.length > 1) {
			colors = tmp;
		} else if(tmp.length == 1) {
			colors = new int[]{tmp[0], tmp[0]};
		} else {
			colors = new int[]{Color.BLACK, Color.BLACK};
		}
		GradientDrawable d = new GradientDrawable(GradientDrawable.Orientation.LEFT_RIGHT, colors);
		d.setShape(GradientDrawable.RECTANGLE);
		d.setCornerRadius(30);
		x.setBackgroundDrawable(d);
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
