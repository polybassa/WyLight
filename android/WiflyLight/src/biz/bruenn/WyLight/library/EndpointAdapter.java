package biz.bruenn.WyLight.library;

import java.util.List;

import biz.bruenn.WiflyLight.R;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.ImageView;
import android.widget.TextView;

public class EndpointAdapter extends ArrayAdapter<Endpoint> {

	public EndpointAdapter(Context context, int textViewResourceId,	List<Endpoint> objects) {
		super(context, textViewResourceId, objects);
	}

	@Override
	public View getView(int position, View convertView, ViewGroup parent) {
		final Endpoint remote = getItem(position);
		View v = LayoutInflater.from(getContext()).inflate(R.layout.view_remote_row, parent, false);
		ImageView status = (ImageView) v.findViewById(R.id.status);
		TextView deviceId = (TextView) v.findViewById(R.id.deviceId);
		TextView address = (TextView) v.findViewById(R.id.address);

		deviceId.setText(remote.getName());
		address.setText(remote.getAddress());

		if(remote.isOnline()) {
			status.setImageResource(R.drawable.ic_status_good);
			status.setContentDescription(getContext().getResources().getString(R.string.status_good));
		} else  {
			status.setImageResource(R.drawable.ic_status_bad);
			status.setContentDescription(getContext().getResources().getString(R.string.status_bad));
		}
		return v;
	}
}
