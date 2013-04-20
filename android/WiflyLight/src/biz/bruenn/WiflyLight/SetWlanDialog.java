package biz.bruenn.WiflyLight;

import biz.bruenn.WiflyLight.exception.FatalError;
import android.app.Dialog;
import android.content.Context;
import android.os.Bundle;
import android.text.InputType;
import android.view.View;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.Toast;

public class SetWlanDialog extends Dialog {
	private final Endpoint mRemote;
	private EditText mPass;
	private EditText mSsid;
	
	SetWlanDialog(Context context, Endpoint remote) {
		super(context);
		mRemote = remote;
	}
	
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.dialog_set_wlan);
		setTitle(R.string.title_dialog_set_wlan);
		
		mPass = (EditText)findViewById(R.id.editPassphrase);
		mSsid = (EditText)findViewById(R.id.editSsid);

		WiflyControl control = new WiflyControl();
		try {
			control.connect(mRemote);
			mSsid.setText(control.confGetSsid());
			control.disconnect();
		} catch (FatalError e) {
			Toast.makeText(getContext(), R.string.msg_connectionfailed, Toast.LENGTH_SHORT).show();
			dismiss();
		}
		
		CheckBox toggle = (CheckBox)findViewById(R.id.togglePassphrase);
		toggle.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
			
			public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
				if(isChecked) {
					mPass.setInputType(InputType.TYPE_CLASS_TEXT | InputType.TYPE_TEXT_VARIATION_VISIBLE_PASSWORD);
				} else {
					mPass.setInputType(InputType.TYPE_CLASS_TEXT | InputType.TYPE_TEXT_VARIATION_PASSWORD);
				}
			}
		});

		Button save = (Button)findViewById(R.id.save);
		save.setOnClickListener(new View.OnClickListener() {
			
			public void onClick(View v) {
				
				WiflyControl control = new WiflyControl();
				try {
					control.connect(mRemote);
					control.confSetWlan(mPass.getText().toString(),	mSsid.getText().toString());
					control.disconnect();
				} catch (FatalError e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				dismiss();
			}
		});
		
		Button cancel = (Button)findViewById(R.id.cancel);
		cancel.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
				cancel();				
			}
		});
	}
}
