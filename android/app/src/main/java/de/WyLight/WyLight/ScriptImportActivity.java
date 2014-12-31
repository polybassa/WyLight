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

import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;

import de.WyLight.WyLight.R;

import android.database.Cursor;
import android.net.Uri;
import android.os.Bundle;
import android.provider.MediaStore;
import android.support.v4.app.FragmentActivity;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

public class ScriptImportActivity extends FragmentActivity {

	private byte[] mScriptData;
	private int mScriptDataLength;
	private EditText mScriptName;

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_import_script);
		Uri uri = getIntent().getData();
		try {
			InputStream in = getContentResolver().openInputStream(uri);
			mScriptData = new byte[10240];
			mScriptDataLength = in.read(mScriptData);
			in.close();
			// TODO read the name from inside the file this here works only with
			// gmail not k-9
			Cursor c = getContentResolver().query(uri, null, null, null, null);
			c.moveToFirst();
			final int column = c
					.getColumnIndex(MediaStore.MediaColumns.DISPLAY_NAME);
			mScriptName = (EditText) this.findViewById(R.id.scriptName);
			if (column >= 0) {
				mScriptName.setText(c.getString(column));
			} else {
				mScriptName.setText("new.wyscript");
			}

		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

		Button cancel = (Button) findViewById(R.id.cancel);
		cancel.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
				finish();
			}
		});
		Button save = (Button) findViewById(R.id.save);
		save.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
				CopyHelper copyHelper = new CopyHelper(getFilesDir()
						.getAbsolutePath());
				try {
					copyHelper.copyBufferToFile(mScriptData, mScriptDataLength,
							mScriptName.getText().toString());
				} catch (FileNotFoundException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				finish();
			}
		});
	}
}
