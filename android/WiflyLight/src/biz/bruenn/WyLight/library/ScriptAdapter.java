package biz.bruenn.WyLight.library;

import biz.bruenn.WyLight.FadeTime;
import android.content.Context;
import android.graphics.Color;
import android.graphics.drawable.GradientDrawable;
import android.util.DisplayMetrics;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.widget.BaseAdapter;
import android.widget.TextView;

public class ScriptAdapter extends BaseAdapter {

	private native void addFade(long pNative, int argb, int addr, short fadeTime);
	private native void addGradient(long pNative, int argb_1, int argb_2, short fadeTime);
	private native void clear(long pNative);
	private native long create(String filename);
	private native long getItem(long pNative, int position);
	private native String name(long pNative);
	private native int numCommands(long pNative);
	private final long mNative;
	public boolean mDesignatedForDeletion = false;
	
	public ScriptAdapter(long pNative) {
		mNative = pNative;
	}
	
	ScriptAdapter(String filename) {
		mNative = create(filename);
	}
	
	public void addFade(int argb, int addr, short fadeTime) {
		addFade(mNative, argb, addr, fadeTime);
		notifyDataSetChanged();
	}

	public void addGradient(int argb_1, int argb_2, short fadeTime) {
		addGradient(mNative, argb_1, argb_2, fadeTime);
		notifyDataSetChanged();
	}
	
	public void clear() {
		clear(mNative);
		notifyDataSetChanged();
	}

	public int[] getColors() {
		int[] colors = new int[getCount()];
		for(int i = 0; i < colors.length; ++i) {
			colors[i] = getItem(i).getColor();
		}
		return colors;
	}

	public int getCount() {
		return numCommands(mNative);
	}

	public FwCmdScriptAdapter getItem(int position) {
		return new FwCmdScriptAdapter(getItem(mNative, position));
	}

	public long getItemId(int position) {
		return getItem(mNative, position);
	}
	
	public String getName() {
		return name(mNative);
	}
	
	public long getNative() {
		return mNative;
	}

	public View getView(int position, View convertView, ViewGroup parent) {
		final FwCmdScriptAdapter item = getItem(position);
		switch(item.getType()) {
		case FADE:
			return getFadeView(position, convertView, parent);
		case GRADIENT:
			return getGradientView(position, convertView, parent);
		default:
			TextView v = new TextView(parent.getContext());
			v.setText("no " + item.getType() + " view available");
			return v;
		}
	}

	private View getFadeView(int position, View convertView, ViewGroup parent) {
		WindowManager wm = (WindowManager)parent.getContext().getSystemService(Context.WINDOW_SERVICE);
		DisplayMetrics metrics = new DisplayMetrics();
		wm.getDefaultDisplay().getMetrics(metrics);
		TextView v = new TextView(parent.getContext());
		final float factor = FadeTime.timeToScaling(getItem(position).getTime());
		v.setHeight((int)(factor * metrics.xdpi));
		v.setWidth(metrics.widthPixels);
		final int topColor = (0 >= position) ? Color.BLACK : getItem(position-1).getColor();
		final int bottomColor = getItem(position).getColor();
		final int colors[] = new int[]{topColor, bottomColor};
		GradientDrawable d = new GradientDrawable(GradientDrawable.Orientation.TOP_BOTTOM, colors);
		d.setShape(GradientDrawable.RECTANGLE);
		d.setCornerRadius(30);
		v.setBackgroundDrawable(d);
		return v;
	}

	private View getGradientView(int position, View convertView, ViewGroup parent) {
		WindowManager wm = (WindowManager)parent.getContext().getSystemService(Context.WINDOW_SERVICE);
		DisplayMetrics metrics = new DisplayMetrics();
		wm.getDefaultDisplay().getMetrics(metrics);
		TextView v = new TextView(parent.getContext());
		final float factor = FadeTime.timeToScaling(getItem(position).getTime());
		v.setHeight((int)(factor * metrics.xdpi));
		v.setWidth(metrics.widthPixels);
		final int colors[] = getItem(position).getGradientColor();
		GradientDrawable d = new GradientDrawable(GradientDrawable.Orientation.LEFT_RIGHT, colors);
		d.setShape(GradientDrawable.RECTANGLE);
		d.setCornerRadius(30);
		v.setBackgroundDrawable(d);
		return v;
	}
}
