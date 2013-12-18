package biz.bruenn.WyLight.library;

import java.util.List;

import biz.bruenn.WiflyLight.R;

import android.content.Context;
import android.graphics.Color;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.TextView;

public class EndpointAdapter extends ArrayAdapter<Endpoint> {

	public EndpointAdapter(Context context, int textViewResourceId,	List<Endpoint> objects) {
		super(context, textViewResourceId, objects);
	}

	@Override
	public View getView(int position, View convertView, ViewGroup parent) {
		final Endpoint remote = getItem(position);
		View v = LayoutInflater.from(getContext()).inflate(R.layout.view_remote_row, parent, false);
		TextView deviceId = (TextView) v.findViewById(R.id.deviceId);
		TextView address = (TextView) v.findViewById(R.id.address);

		deviceId.setText(remote.getName());
		address.setText(remote.getAddress());

		if(remote.isOnline()) {
			v.setBackgroundColor(Color.WHITE);
			deviceId.setTextColor(getContext().getResources().getColor(android.R.color.primary_text_light));
			address.setTextColor(getContext().getResources().getColor(android.R.color.secondary_text_light));
		}
		return v;
	}
}
