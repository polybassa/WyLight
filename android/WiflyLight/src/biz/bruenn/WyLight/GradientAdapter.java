package biz.bruenn.WyLight;

import java.util.List;

import android.content.Context;
import android.graphics.LinearGradient;
import android.graphics.Shader;
import android.graphics.drawable.ShapeDrawable;
import android.graphics.drawable.shapes.RectShape;
import android.util.DisplayMetrics;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.TextView;


public class GradientAdapter extends ArrayAdapter<GradientValue> {
	
	public GradientAdapter(Context context, int textViewResourceId,
			List<GradientValue> objects) {
		super(context, textViewResourceId, objects);
	}
	
	@Override
	public View getView(int position, View convertView, ViewGroup parent) {
		//TODO optimize performance
		TextView text = new TextView(parent.getContext());
		DisplayMetrics metrics = getContext().getResources().getDisplayMetrics();
		final float height = (40f * metrics.density + 0.5f);
		text.setHeight((int)height);
		ShapeDrawable background = new ShapeDrawable();
		RectShape s = new RectShape();
		int firstColor = getItem(Math.max(0, position - 1)).getValue();
		int secondColor = getItem(position).getValue(); 
		LinearGradient lg = new LinearGradient(0, 0, 0, height, new int[]{firstColor, secondColor}, new float[]{0, 1}, Shader.TileMode.CLAMP);
		background.setShape(s);
		background.getPaint().setShader(lg);		
		text.setBackgroundDrawable(background);
		return text;
	}
}
