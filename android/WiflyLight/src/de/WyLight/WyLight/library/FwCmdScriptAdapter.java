package de.WyLight.WyLight.library;

import android.content.Context;
import android.graphics.Color;
import android.graphics.drawable.GradientDrawable;
import android.view.View;
import android.widget.TextView;

public class FwCmdScriptAdapter {

	private native int getFadeColor(long pNative);

	private native int getFadeTime(long pNative);

	private native long getGradientColors(long pNative);

	private native String getToken(long pNative);

	private native char getType(long pNative);

	private native int setFadeColor(long pNative, int argb);

	private native int setGradientColors(long pNative, int startArgb,
			int endArgb);

	private native int setFadeTime(long pNative, short time);

	private final long mNative;

	public enum Type {
		UNKOWN, FADE, GRADIENT
	}

	public FwCmdScriptAdapter(long pNative) {
		mNative = pNative;
	}

	public int getColor() {
		if (0 != mNative) {
			if (getType() == Type.GRADIENT) {
				final int[] colors = getGradientColor();
				final int r = Color.red(colors[0]) + Color.red(colors[1]);
				final int g = Color.green(colors[0]) + Color.green(colors[1]);
				final int b = Color.blue(colors[0]) + Color.blue(colors[1]);
				return Color.rgb(r / 2, g / 2, b / 2);
			}
			return getFadeColor(mNative);
		}
		return 0;
	}

	public int[] getGradientColor() {
		int[] temp = new int[2];
		if (0 != mNative) {
			long dual = getGradientColors(mNative);
			temp[0] = (int) (dual & 0xffffffff);
			temp[1] = (int) (dual >> 32);
		}
		return temp;
	}

	public int getTime() {
		if (0 != mNative) {
			return getFadeTime(mNative);
		}
		return 0;
	}

	public Type getType() {
		if (0 != mNative) {
			final char type = getType(mNative);
			if (0xF9 == type) {
				return Type.GRADIENT;
			} else if (0xFC == type) {
				return Type.FADE;
			}
		}
		return Type.UNKOWN;
	}

	public View getView(Context context, int width, int height,
			int colorNeighbor) {
		final TextView v = new TextView(context);
		final GradientDrawable d;
		if (getType() == Type.GRADIENT) {
			d = new GradientDrawable(GradientDrawable.Orientation.TOP_BOTTOM,
					getGradientColor());
		} else {
			final int[] colors = new int[] { colorNeighbor, getColor() };
			d = new GradientDrawable(GradientDrawable.Orientation.LEFT_RIGHT,
					colors);
		}
		v.setBackgroundDrawable(d);
		v.setWidth(width);
		v.setHeight(height);
		return v;
	}

	public void setColor(int argb) {
		if (0 != mNative) {
			setFadeColor(mNative, argb);
		}
	}

	public void setGradientColors(int startArgb, int endArgb) {
		if (0 != mNative) {
			setGradientColors(mNative, startArgb, endArgb);
		}
	}

	public void setTime(short time) {
		if (0 != mNative) {
			setFadeTime(mNative, time);
		}
	}
}
