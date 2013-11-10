package biz.bruenn.WyLight;

import java.util.ArrayList;
import java.util.Random;

import biz.bruenn.WiflyLight.R;
import biz.bruenn.WyLight.exception.ConnectionTimeout;
import biz.bruenn.WyLight.exception.FatalError;
import biz.bruenn.WyLight.exception.ScriptBufferFull;
import biz.bruenn.WyLight.library.ScriptManagerAdapter;
import android.content.Intent;
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

	private ArrayList<ScriptCommand> mArrayList = new ArrayList<ScriptCommand>();
	private ScriptCommandAdapter mAdapter;
	private ScriptManagerAdapter mScriptList;
	private Random r = new Random();

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup group, Bundle savedInstanceState) {
		View v = inflater.inflate(R.layout.fragment_scripting, group, false);
		
		mAdapter = new ScriptCommandAdapter(this.getActivity(), android.R.layout.simple_list_item_1, mArrayList);
		mAdapter.notifyDataSetChanged();
		ListView list = (ListView)v.findViewById(R.id.scriptList);
		list.setAdapter(mAdapter);
		list.setOnItemClickListener(new AdapterView.OnItemClickListener() {

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
				mAdapter.add(new ScriptCommand(String.valueOf(r.nextInt())));
			}
		});
		
		Button clear = (Button)v.findViewById(R.id.clear);
		clear.setOnClickListener(new View.OnClickListener() {
			
			public void onClick(View v) {
				mAdapter.clear();
				mProvider.getControl().fwClearScript();
			}
		});
		
		Button newScript = (Button)v.findViewById(R.id.new_script);
		newScript.setOnClickListener(new View.OnClickListener() {
			
			public void onClick(View v) {
				mScriptList.add("Script" + Math.random() * 1000);
			}
		});
		
		Button send = (Button)v.findViewById(R.id.send);
		send.setOnClickListener(new View.OnClickListener() {
			
			public void onClick(View v) {
				sendScript();				
			}
		});
		
		Spinner savedScripts = (Spinner)v.findViewById(R.id.savedScripts);
		mScriptList = new ScriptManagerAdapter(getActivity().getApplicationContext());
		savedScripts.setAdapter(mScriptList);
		
		return v;
	}
	
	@Override
	public void onActivityResult(int requestCode, int resultCode, Intent data) {
		if(null != data) {
			final int position = data.getIntExtra(ITEM_POSITION, 0);
			mAdapter.getItem(position).setColor(resultCode);
			mAdapter.notifyDataSetChanged();
		}
	}
	
	private void sendScript() {
		try {
			mProvider.getControl().fwClearScript();
			mProvider.getControl().fwLoopOn();
			for(ScriptCommand cmd: mArrayList) {
				cmd.sendTo(mProvider.getControl());
			}
			mProvider.getControl().fwLoopOff((byte)0);
		} catch (ConnectionTimeout e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (ScriptBufferFull e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (FatalError e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
