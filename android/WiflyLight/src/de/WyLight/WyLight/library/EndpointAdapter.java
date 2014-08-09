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

import java.util.List;

import de.WyLight.WyLight.R;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.ImageView;
import android.widget.TextView;

public class EndpointAdapter extends ArrayAdapter<Endpoint> {
	private static final boolean SHOW_PORT = false;

	public EndpointAdapter(Context context, int textViewResourceId,
			List<Endpoint> objects) {
		super(context, textViewResourceId, objects);
	}

	@Override
	public View getView(int position, View convertView, ViewGroup parent) {
		final Endpoint remote = getItem(position);
		View v = LayoutInflater.from(getContext()).inflate(
				R.layout.view_remote_row, parent, false);
		ImageView status = (ImageView) v.findViewById(R.id.status);
		TextView deviceId = (TextView) v.findViewById(R.id.deviceId);
		TextView address = (TextView) v.findViewById(R.id.address);

		deviceId.setText(remote.getName());
		if (SHOW_PORT) {
			address.setText(remote.getAddressPort());
		} else {
			address.setText(remote.getAddress());
		}

		if (remote.isOnline()) {
			status.setImageResource(R.drawable.ic_status_good);
			status.setContentDescription(getContext().getResources().getString(
					R.string.status_good));
		} else {
			status.setImageResource(R.drawable.ic_status_bad);
			status.setContentDescription(getContext().getResources().getString(
					R.string.status_bad));
		}
		return v;
	}
}
