package biz.bruenn.WyLight.view;

import biz.bruenn.WiflyLight.R;
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
		mText = (TextView)v.findViewById(R.id.colorField);
		mColorStatus = (GradientDrawable) mText.getBackground();
		mShowText = attrib.getAttributeBooleanValue("http://schemas.android.com/apk/res-auto", "showText", false);
		final int color = attrib.getAttributeIntValue("http://schemas.android.com/apk/res-auto", "color", Color.BLACK);
		setColor(color);
		setText(attrib.getAttributeValue("http://schemas.android.com/apk/res-auto", "text"));
	}

	public int getColor() {
		return mColor;
	}

	public void setColor(int color) {
		mColor = color;
		mColorStatus.setColor(color);
		mText.setTextColor(0xff000000 | ~color);
	}

	public void setGradient(GradientDrawable d) {
		mText.setBackgroundDrawable(d);
	}

	public void setText(String text) {
		if(mShowText) {
			mText.setText(text);
		}
	}
}
