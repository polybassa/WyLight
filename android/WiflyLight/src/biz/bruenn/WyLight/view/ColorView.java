package biz.bruenn.WyLight.view;

import biz.bruenn.WiflyLight.R;
import android.content.Context;
import android.graphics.drawable.GradientDrawable;
import android.util.AttributeSet;
import android.view.View;
import android.widget.RelativeLayout;



public class ColorView extends RelativeLayout {

	private final GradientDrawable mColorStatus;

	public ColorView(Context context, AttributeSet attrib) {
		super(context, attrib);
		final View v = View.inflate(context, R.layout.view_color, this);
		mColorStatus = (GradientDrawable) v.findViewById(R.id.colorField).getBackground();
	}

	public void setColor(int color) {
		mColorStatus.setColor(color);
	}
}
