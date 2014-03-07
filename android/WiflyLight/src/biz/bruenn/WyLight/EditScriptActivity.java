package biz.bruenn.WyLight;

import biz.bruenn.WiflyLight.R;
import biz.bruenn.WyLight.library.ScriptAdapter;
import biz.bruenn.WyLight.library.ScriptManagerAdapter;
import android.app.Activity;
import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.ListView;

public class EditScriptActivity extends Activity {
	public static final String ITEM_POSITION = "ITEM_POSITION";
	public static final String NATIVE_SCRIPT = "NATIVE_SCRIPT";

	private ListView mCommandList;
	private ScriptAdapter mScript;

	@Override
	public void onActivityResult(int requestCode, int resultCode, Intent data) {
		if(null != data) {
			final int position = data.getIntExtra(ITEM_POSITION, 0);
			mScript.getItem(position).setColor(resultCode);
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
		mCommandList = (ListView)findViewById(R.id.scriptList);
		mCommandList.setAdapter(mScript);
		mCommandList.setOnItemClickListener(new AdapterView.OnItemClickListener() {
			public void onItemClick(AdapterView<?> parent, View v, int position,
					long id) {
				Intent i = new Intent(v.getContext(), EditCommandActivity.class);
				i.putExtra(ITEM_POSITION, position);
				startActivityForResult(i, 0);
			}
		});

		Button add = (Button)findViewById(R.id.addCommand);
		add.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
				mScript.addFade(Color.WHITE, 0xffffffff, (short)500);
				new ScriptManagerAdapter(getBaseContext()).save(mScript);
			}
		});

		Button clear = (Button)findViewById(R.id.clear);
		clear.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
				mScript.clear();
			}
		});
	}
}
