package biz.bruenn.WyLight;

import java.text.SimpleDateFormat;
import java.util.Date;

import biz.bruenn.WiflyLight.R;
import biz.bruenn.WyLight.library.ScriptAdapter;
import biz.bruenn.WyLight.library.ScriptManagerAdapter;
import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.ListView;

public class SetScriptFragment extends ControlFragment {
	public static final String ITEM_POSITION = "ITEM_POSITION";

	private ScriptAdapter mScript= null;
	private ListView mScriptList;
	private ScriptManagerAdapter mScriptListAdapter;

	private void addNewScript()
	{
		String now = new SimpleDateFormat("yyyyMMdd-HHmmss", java.util.Locale.GERMANY).format(new Date());
		mScriptListAdapter.add(now);
		mScriptList.setSelection(mScriptListAdapter.getCount() - 1);
	}

	@Override
	public int getIcon() {
		return R.drawable.ic_action_settings;
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup group, Bundle savedInstanceState) {
		View v = inflater.inflate(R.layout.fragment_set_script, group, false);

		mScriptList = (ListView)v.findViewById(R.id.savedScripts);
		mScriptListAdapter = new ScriptManagerAdapter(getActivity().getApplicationContext());
		mScriptList.setAdapter(mScriptListAdapter);
		mScriptList.setOnItemClickListener(new AdapterView.OnItemClickListener() {
			public void onItemClick(AdapterView<?> parent, View v, int position,
					long arg3) {
				mScript = mScriptListAdapter.getItem(position);
				Intent i = new Intent(v.getContext(), EditScriptActivity.class);
				i.putExtra(EditScriptActivity.NATIVE_SCRIPT, mScriptListAdapter.getItem(position).getNative());
				startActivityForResult(i, 0);
			}
		});
		mScriptList.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
			public void onItemSelected(AdapterView<?> parent, View view,
					int position, long id) {
				mScript = mScriptListAdapter.getItem(position);
			}

			public void onNothingSelected(AdapterView<?> parent) {
				if(mScriptListAdapter.getCount() < 1) {
					addNewScript();
				}
			}
		});

		Button newScript = (Button)v.findViewById(R.id.new_script);
		newScript.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
				addNewScript();
			}
		});
		Button send = (Button)v.findViewById(R.id.send);
		send.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
				if(mScript != null) {
					mProvider.sendScript(mScript);
				}
			}
		});
		return v;
	}
}
