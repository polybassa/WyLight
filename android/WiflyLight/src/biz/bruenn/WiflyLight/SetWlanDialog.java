package biz.bruenn.WiflyLight;

import android.app.Dialog;
import android.content.Context;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

public class SetWlanDialog extends Dialog {
	private final WiflyControl mControl;
	
	SetWlanDialog(Context context, Endpoint remote) {
		super(context);
		mControl = new WiflyControl(remote.getAddr(), remote.getPort());
	}
	
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.dialog_set_wlan);
		
		Button save = (Button)findViewById(R.id.save);
		save.setOnClickListener(new View.OnClickListener() {
			
			public void onClick(View v) {
				// TODO replace this dummy call with confSetWlan()
				mControl.fwSetColor(0xffffffff, 0xff000000);
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
