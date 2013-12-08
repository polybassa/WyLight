package biz.bruenn.WyLight;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;

import biz.bruenn.WiflyLight.R;

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
			//TODO read the name from inside the file this here works only with gmail not k-9
			Cursor c = getContentResolver().query(uri, null, null, null, null);
			c.moveToFirst();
			final int column = c.getColumnIndex(MediaStore.MediaColumns.DISPLAY_NAME);
			mScriptName = (EditText)this.findViewById(R.id.scriptName);
			if(column >= 0) {
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

		Button cancel = (Button)findViewById(R.id.cancel);
		cancel.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
				finish();
			}
		});
		Button save = (Button)findViewById(R.id.save);
		save.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
				CopyHelper copyHelper = new CopyHelper(getFilesDir().getAbsolutePath());
				try {
					copyHelper.copyBufferToFile(mScriptData, mScriptDataLength, mScriptName.getText().toString());
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
