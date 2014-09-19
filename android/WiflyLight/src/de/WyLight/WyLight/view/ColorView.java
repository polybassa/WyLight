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

package de.WyLight.WyLight.view;

import de.WyLight.WyLight.R;
import android.content.Context;
import android.graphics.Color;
import android.graphics.drawable.GradientDrawable;
import android.util.AttributeSet;
import android.view.View;
import android.widget.RelativeLayout;
import android.widget.TextView;

public class ColorView extends RelativeLayout {

	private final GradientDrawable mColorStatus;
	private final TextView mText;
	private final boolean mShowText;
	private int mColor;

	public ColorView(Context context, AttributeSet attrib) {
		super(context, attrib);
		final View v = View.inflate(context, R.layout.view_color, this);
		mText = (TextView) v.findViewById(R.id.colorField);
		mColorStatus = (GradientDrawable) mText.getBackground();
		mShowText = attrib.getAttributeBooleanValue(
				"http://schemas.android.com/apk/res-auto", "showText", false);
		final int color = attrib
				.getAttributeIntValue(
						"http://schemas.android.com/apk/res-auto", "color",
						Color.BLACK);
		setColor(color);
		setText(attrib.getAttributeValue(
				"http://schemas.android.com/apk/res-auto", "text"));
	}

	public int getColor() {
		return mColor;
	}

	public void setColor(int color) {
		mColor = color;
		mColorStatus.setColor(color);
		mText.setTextColor(0xff000000 | ~color);
	}

	public void setGradient(int[] colors) {
		//TODO switch to API 16: mColorStatus.setColors(colors);
		final GradientDrawable d;
		if (1 == colors.length) {
			d = new GradientDrawable(
					GradientDrawable.Orientation.LEFT_RIGHT,
					new int[] {colors[0], colors[0] });
		} else {
			d = new GradientDrawable(
					GradientDrawable.Orientation.LEFT_RIGHT,
					colors);
		}
		d.setCornerRadius(8f);
		mText.setBackgroundDrawable(d);
	}

	public void setText(String text) {
		if (mShowText) {
			mText.setText(text);
		}
	}
}
