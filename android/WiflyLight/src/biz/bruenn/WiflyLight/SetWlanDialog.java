package biz.bruenn.WiflyLight;

import android.app.Dialog;
import android.content.Context;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

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
		
		mPass = (EditText)findViewById(R.id.editPassphrase);
		mSsid = (EditText)findViewById(R.id.editSsid);

		WiflyControl control = new WiflyControl();
		control.connect(mRemote);
		mSsid.setText(control.confGetSsid());
		control.disconnect();
		
		Button save = (Button)findViewById(R.id.save);
		save.setOnClickListener(new View.OnClickListener() {
			
			public void onClick(View v) {
				
				WiflyControl control = new WiflyControl();
				control.connect(mRemote);
				control.confSetWlan(mPass.getText().toString(),	mSsid.getText().toString());
				control.disconnect();
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
