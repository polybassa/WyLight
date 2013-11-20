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
import android.widget.Spinner;

public class ScriptingFragment extends ControlFragment {
	public static final String ITEM_POSITION = "ITEM_POSITION";

	private ScriptManagerAdapter mScriptList;
	private ListView mCommandList;

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup group, Bundle savedInstanceState) {
		View v = inflater.inflate(R.layout.fragment_scripting, group, false);
			
		Spinner savedScripts = (Spinner)v.findViewById(R.id.savedScripts);
		mScriptList = new ScriptManagerAdapter(getActivity().getApplicationContext());
		savedScripts.setAdapter(mScriptList);
		savedScripts.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
			public void onItemSelected(AdapterView<?> parent, View view,
					int position, long id) {
				mScriptList.save(scriptAdapter());
				mCommandList.setAdapter(mScriptList.getItem(position));				
			}

			public void onNothingSelected(AdapterView<?> parent) {
				// TODO Auto-generated method stub
			}
		});	
		
		mCommandList = (ListView)v.findViewById(R.id.scriptList);
		mCommandList.setOnItemClickListener(new AdapterView.OnItemClickListener() {
			public void onItemClick(AdapterView<?> parent, View v, int position,
					long id) {
				Intent i = new Intent(v.getContext(), EditCommandActivity.class);
				i.putExtra(ITEM_POSITION, position);
				startActivityForResult(i, 0);
			}
			
		});
		
		Button add = (Button)v.findViewById(R.id.addCommand);
		add.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
				scriptAdapter().addFade(Color.GREEN, 0xffffffff, (short)500);
			}
		});
		
		Button clear = (Button)v.findViewById(R.id.clear);
		clear.setOnClickListener(new View.OnClickListener() {
			
			public void onClick(View v) {
				scriptAdapter().clear();
				mProvider.getControl().fwClearScript();
			}
		});
		
		Button newScript = (Button)v.findViewById(R.id.new_script);
		newScript.setOnClickListener(new View.OnClickListener() {	
			public void onClick(View v) {
				String now = new SimpleDateFormat("yyyyMMdd-HHmmss", java.util.Locale.GERMANY).format(new Date());
				mScriptList.add(now);
			}
		});
		
		Button send = (Button)v.findViewById(R.id.send);
		send.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
				onSendScript(scriptAdapter());
			}
		});
		
		return v;
	}
	
	@Override
	public void onActivityResult(int requestCode, int resultCode, Intent data) {
		if(null != data) {
			final int position = data.getIntExtra(ITEM_POSITION, 0);
			scriptAdapter().getItem(position).setColor(resultCode);
			scriptAdapter().notifyDataSetChanged();
			mScriptList.save(scriptAdapter());
		}
	}
	
	@Override
	public void onPause() {
		super.onPause();
		mScriptList.save(scriptAdapter());
	}
	
	private ScriptAdapter scriptAdapter() {
		return (ScriptAdapter)mCommandList.getAdapter();
	}
}
