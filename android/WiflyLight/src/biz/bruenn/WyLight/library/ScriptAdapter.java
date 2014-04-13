package biz.bruenn.WyLight.library;

import biz.bruenn.WyLight.FadeTime;
import android.content.Context;
import android.graphics.Color;
import android.graphics.CornerPathEffect;
import android.graphics.drawable.GradientDrawable;
import android.graphics.drawable.ShapeDrawable;
import android.graphics.drawable.shapes.RectShape;
import android.util.DisplayMetrics;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.widget.BaseAdapter;
import android.widget.LinearLayout;
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
			return getView(item, parent.getContext(), getFadeBackground(item, position));
		case GRADIENT:
			return getView(item, parent.getContext(), getGradientBackground(item));
		default:
			TextView v = new TextView(parent.getContext());
			v.setText("no " + item.getType() + " view available");
			return v;
		}
	}

	private View getView(FwCmdScriptAdapter item, Context context, GradientDrawable d) {
		WindowManager wm = (WindowManager)context.getSystemService(Context.WINDOW_SERVICE);
		DisplayMetrics metrics = new DisplayMetrics();
		wm.getDefaultDisplay().getMetrics(metrics);
		TextView v = new TextView(context);
		final float factor = FadeTime.timeToScaling(item.getTime());
		v.setHeight((int)(factor * metrics.xdpi));
		v.setWidth(metrics.widthPixels);
		d.setShape(GradientDrawable.RECTANGLE);
		d.setCornerRadius(30);
		v.setBackgroundDrawable(d);
		return v;
	}

	public View getView(Context context) {
		WindowManager wm = (WindowManager)context.getSystemService(Context.WINDOW_SERVICE);
		DisplayMetrics metrics = new DisplayMetrics();
		wm.getDefaultDisplay().getMetrics(metrics);
		final int count = getCount();
		final int height = (int)(0.5f * metrics.xdpi);
		if (count > 0) {
			final int width = metrics.widthPixels / count;
			LinearLayout l = new LinearLayout(context);
			l.setOrientation(LinearLayout.HORIZONTAL);
			int lastColor = Color.BLACK;
			for (int i = 0; i < count; ++i) {
				FwCmdScriptAdapter item = getItem(i);
				l.addView(item.getView(context, width, height, lastColor));
				lastColor = item.getColor();
			}
			ShapeDrawable background = new ShapeDrawable();
			background.setShape(new RectShape());
			background.getPaint().setPathEffect(new CornerPathEffect(100));
			l.setBackgroundDrawable(background);
			return l;
		}
		TextView empty = new TextView(context);
		empty.setHeight(height);
		empty.setWidth(metrics.widthPixels);
		empty.setBackgroundColor(Color.BLACK);
		return empty;
	}

	private GradientDrawable getFadeBackground(FwCmdScriptAdapter item, final int position) {
		final int topColor = (position <= 0) ? Color.BLACK : getItem(position-1).getColor();
		final int bottomColor = item.getColor();
		final int colors[] = new int[]{topColor, bottomColor};
		return new GradientDrawable(GradientDrawable.Orientation.TOP_BOTTOM, colors);
	}

	private GradientDrawable getGradientBackground(FwCmdScriptAdapter item) {
		final int colors[] = item.getGradientColor();
		return new GradientDrawable(GradientDrawable.Orientation.LEFT_RIGHT, colors);
	}
}
