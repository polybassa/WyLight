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

import de.WyLight.WyLight.R;
import de.WyLight.WyLight.library.FwCmdScriptAdapter;
import de.WyLight.WyLight.library.FwCmdScriptAdapter.Type;
import de.WyLight.WyLight.library.ScriptAdapter;
import de.WyLight.WyLight.library.ScriptManagerAdapter;
import android.app.Activity;
import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ListView;

public class EditScriptActivity extends Activity {
	public static final int DO_DELETE = -1;
	public static final String ITEM_COLOR = "ITEM_COLOR";
	public static final String ITEM_COLORS = "ITEM_COLORS";
	public static final String ITEM_POSITION = "ITEM_POSITION";
	public static final String ITEM_TIME = "ITEM_TIME";
	public static final String NATIVE_SCRIPT = "NATIVE_SCRIPT";

	private ListView mCommandList;
	private ScriptAdapter mScript;

	@Override
	public void onActivityResult(int requestCode, int resultCode, Intent data) {
		if (null != data) {
			final int position = data.getIntExtra(ITEM_POSITION, 0);
			final int time = data.getIntExtra(ITEM_TIME, 0);
			final FwCmdScriptAdapter cmd = mScript.getItem(position);
			if (cmd.getType() == Type.GRADIENT) {
				final int[] colors = data.getIntArrayExtra(ITEM_COLORS);
				cmd.setGradientColors(colors[0], colors[1]);
			} else {
				final int[] colors = data.getIntArrayExtra(ITEM_COLORS);
				cmd.setColor(colors[0]);
			}
			cmd.setTime((short) time);
			mScript.notifyDataSetChanged();
			new ScriptManagerAdapter(getBaseContext()).save(mScript);
		}
	}

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_edit_script);

		final long mNative = getIntent().getLongExtra(NATIVE_SCRIPT, 0);
		mScript = new ScriptAdapter(mNative);
		mCommandList = (ListView) findViewById(R.id.scriptList);
		mCommandList.setAdapter(mScript);
		mCommandList
				.setOnItemClickListener(new AdapterView.OnItemClickListener() {
					public void onItemClick(AdapterView<?> parent, View v,
							int position, long id) {
						final FwCmdScriptAdapter cmd = mScript
								.getItem(position);
						final Intent i = new Intent(v.getContext(),
								EditGradientActivity.class);
						if (cmd.getType() == Type.GRADIENT) {
							i.putExtra(ITEM_COLORS, cmd.getGradientColor());
						} else {
							i.putExtra(ITEM_COLORS,
									new int[] { cmd.getColor() });
						}
						i.putExtra(ITEM_POSITION, position);
						i.putExtra(ITEM_TIME, cmd.getTime());
						startActivityForResult(i, 0);
					}
				});
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		MenuInflater inflater = getMenuInflater();
		inflater.inflate(R.menu.edit_script_activity_actions, menu);
		return super.onCreateOptionsMenu(menu);
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		switch (item.getItemId()) {
		case R.id.action_add:
			mScript.addFade(Color.WHITE, 0xffffffff, (short) 500);
			new ScriptManagerAdapter(getBaseContext()).save(mScript);
			return true;
		case R.id.action_add_gradient:
			mScript.addGradient(Color.WHITE, Color.BLACK, (short) 500);
			new ScriptManagerAdapter(getBaseContext()).save(mScript);
			return true;
		case R.id.action_delete:
			Activity a = (null == getParent()) ? this : getParent();
			a.setResult(EditScriptActivity.DO_DELETE, getIntent());
			finish();
			return true;
		default:
			return super.onOptionsItemSelected(item);
		}
	}
}
