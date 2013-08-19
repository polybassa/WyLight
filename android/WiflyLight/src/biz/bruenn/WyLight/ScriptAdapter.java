package biz.bruenn.WyLight;

import java.util.List;

import android.content.Context;
import android.util.DisplayMetrics;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.TextView;


public class ScriptAdapter extends ArrayAdapter<ScriptCommand> {

	public ScriptAdapter(Context context, int textViewResourceId,
			List<ScriptCommand> objects) {
		super(context, textViewResourceId, objects);
	}
	
	@Override
	public View getView(int position, View convertView, ViewGroup parent) {
		TextView text = new TextView(parent.getContext());
		DisplayMetrics metrics = getContext().getResources().getDisplayMetrics();
		text.setHeight((int)(40f * metrics.density + 0.5f));
		text.setText(String.valueOf(position) + ':');
		text.setBackgroundColor(getItem(position).getColor());
		return text;
	}
	

}
