package biz.bruenn.WyLight;

import java.util.ArrayList;

import biz.bruenn.WiflyLight.R;
import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.ListView;

public class SetGradientFragment extends ControlFragment {
	public static final String ITEM_POSITION = "ITEM_POSITION";

	private ArrayList<GradientValue> mArrayList = new ArrayList<GradientValue>();
	private GradientAdapter mAdapter;

	@Override
	public int getIcon() {
		return R.drawable.ic_action_edit;
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup group, Bundle savedInstanceState) {
		View v = inflater.inflate(R.layout.fragment_scripting, group, false);
		
		mAdapter = new GradientAdapter(this.getActivity(), android.R.layout.simple_list_item_1, mArrayList);
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
				mAdapter.add(new GradientValue(0xff000000));
			}
		});
		
		Button clear = (Button)v.findViewById(R.id.clear);
		clear.setOnClickListener(new View.OnClickListener() {
			
			public void onClick(View v) {
				mAdapter.clear();
				mProvider.getControl().fwClearScript();
			}
		});
		
		Button send = (Button)v.findViewById(R.id.send);
		send.setBackgroundColor(Color.GRAY);
		send.setOnClickListener(new View.OnClickListener() {
			
			public void onClick(View v) {
				final int numGradients = Math.max(1, mArrayList.size());
				final int length = 17 /numGradients;
				int index = 1;
				for(GradientValue color: mArrayList) {
					onSetGradient(color.getValue(), mArrayList.get(Math.min(index, numGradients-1)).getValue(), length, (index-1)*length, (short)1);
					++index;
				}			
			}
		});
		return v;
	}
	
	@Override
	public void onActivityResult(int requestCode, int resultCode, Intent data) {
		if(null != data) {
			final int position = data.getIntExtra(ITEM_POSITION, 0);
			mAdapter.getItem(position).setValue(resultCode);
			mAdapter.notifyDataSetChanged();
		}
	}
}
