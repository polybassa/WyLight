package biz.bruenn.WyLight.view;

import biz.bruenn.WiflyLight.R;
import android.content.Context;
import android.graphics.drawable.GradientDrawable;
import android.util.AttributeSet;
import android.view.View;
import android.widget.RelativeLayout;
import android.widget.TextView;



public class ColorView extends RelativeLayout {

	private final GradientDrawable mColorStatus;
	private final TextView mText;
	private final boolean mShowText;

	public ColorView(Context context, AttributeSet attrib) {
		super(context, attrib);
		final View v = View.inflate(context, R.layout.view_color, this);
		mText = (TextView)v.findViewById(R.id.colorField);
		mColorStatus = (GradientDrawable) mText.getBackground();
		mShowText = attrib.getAttributeBooleanValue("http://schemas.android.com/apk/res-auto", "showText", false);
	}

	public void setColor(int color) {
		mColorStatus.setColor(color);
	}

	public void setText(String text) {
		if(mShowText) {
			mText.setText(text);
		}
	}
}