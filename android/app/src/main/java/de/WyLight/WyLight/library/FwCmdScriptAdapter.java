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

package de.WyLight.WyLight.library;

import android.content.Context;
import android.graphics.Color;
import android.graphics.drawable.GradientDrawable;
import android.graphics.drawable.GradientDrawable.Orientation;
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
			switch (getType(mNative)) {
			case 0xF9:
				return Type.GRADIENT;
			case 0xFC:
				return Type.FADE;
			}
		}
		return Type.UNKOWN;
	}

	public View getView(Context context, int width, int height,	int colorNeighbor) {
		final Orientation orientation;
		final int[] colors;
		if (getType() == Type.GRADIENT) {
			orientation = GradientDrawable.Orientation.TOP_BOTTOM;
			colors = getGradientColor();
		} else {
			orientation = GradientDrawable.Orientation.LEFT_RIGHT;
			colors = new int[] { colorNeighbor, getColor() };
		}
		final TextView v = new TextView(context);
		v.setBackgroundDrawable(new GradientDrawable(orientation, colors));
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
