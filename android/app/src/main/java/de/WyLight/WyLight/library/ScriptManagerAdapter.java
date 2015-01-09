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

import java.io.File;

import android.content.Context;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;

public class ScriptManagerAdapter extends BaseAdapter {

	private native String getScriptName(String path, int index);

	private native int numScripts(String path);

	private native void newScript(String path, String name);

	private native void saveScript(String path, long pNative);

	private final String mPath;

	public ScriptManagerAdapter(Context context) {
		mPath = context.getFilesDir().getAbsolutePath();
	}

	public void add(String scriptName) {
		newScript(mPath + '/' + scriptName + ".wyscript", scriptName);
		notifyDataSetChanged();
	}

	public int getCount() {
		return numScripts(mPath);
	}

	public ScriptAdapter getItem(int position) {
		return new ScriptAdapter(mPath + '/' + getScriptName(mPath, position));
	}

	public long getItemId(int position) {
		return getItem(position).getNative();
	}

	public View getView(int position, View convertView, ViewGroup parent) {
		return getItem(position).getView(parent.getContext());
	}

	public void remove(ScriptAdapter script) {
		new File(mPath + '/' + script.getName()).delete();
		notifyDataSetChanged();
	}

	public void save(ScriptAdapter script) {
		if (null != script && !script.mDesignatedForDeletion) {
			final String fullPath = mPath + '/' + script.getName();
			saveScript(fullPath, script.getNative());
		}
	}
}
