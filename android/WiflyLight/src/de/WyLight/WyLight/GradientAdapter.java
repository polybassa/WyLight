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

package de.WyLight.WyLight;

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
		// TODO optimize performance
		TextView text = new TextView(parent.getContext());
		DisplayMetrics metrics = getContext().getResources()
				.getDisplayMetrics();
		final float height = (40f * metrics.density + 0.5f);
		text.setHeight((int) height);
		ShapeDrawable background = new ShapeDrawable();
		RectShape s = new RectShape();
		int firstColor = getItem(Math.max(0, position - 1)).getValue();
		int secondColor = getItem(position).getValue();
		LinearGradient lg = new LinearGradient(0, 0, 0, height, new int[] {
				firstColor, secondColor }, new float[] { 0, 1 },
				Shader.TileMode.CLAMP);
		background.setShape(s);
		background.getPaint().setShader(lg);
		text.setBackgroundDrawable(background);
		return text;
	}
}
