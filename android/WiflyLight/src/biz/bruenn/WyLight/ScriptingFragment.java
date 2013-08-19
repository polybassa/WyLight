package biz.bruenn.WyLight;

import java.util.ArrayList;
import java.util.Random;

import biz.bruenn.WiflyLight.R;
import android.content.Intent;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.Toast;

public class ScriptingFragment extends ControlFragment {
	public static final String ITEM_POSITION = "ITEM_POSITION";

	private ArrayList<ScriptCommand> mArrayList = new ArrayList<ScriptCommand>();
	private ScriptAdapter mAdapter;
	private Random r = new Random();

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup group, Bundle savedInstanceState) {
		View v = inflater.inflate(R.layout.fragment_scripting, group, false);
		
		mAdapter = new ScriptAdapter(this.getActivity(), android.R.layout.simple_list_item_1, mArrayList);
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
			}
		});
		
		Button send = (Button)v.findViewById(R.id.send);
		send.setOnClickListener(new View.OnClickListener() {
			
			public void onClick(View v) {
				Toast.makeText(getActivity(), "send script not implemented yet", Toast.LENGTH_SHORT).show();
			}
		});
		return v;
	}
	
	@Override
	public void onActivityResult(int requestCode, int resultCode, Intent data) {
		if(null != data) {
			int position = data.getIntExtra(ITEM_POSITION, 0);
			Toast.makeText(getActivity(), String.valueOf(position) + ':' + String.valueOf(resultCode), Toast.LENGTH_SHORT).show();
			mAdapter.getItem(position).setColor(resultCode);
			mAdapter.notifyDataSetChanged();
		}
	}
}
