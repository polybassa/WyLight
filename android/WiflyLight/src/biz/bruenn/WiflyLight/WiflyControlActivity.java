package biz.bruenn.WiflyLight;

import android.os.Bundle;
import android.app.Activity;
import android.content.Intent;
import android.view.Menu;
import android.widget.Toast;

public class WiflyControlActivity extends Activity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_wifly_control);
		Intent i = getIntent();
		int ip = i.getIntExtra("IpAddress", 0);
		short port = i.getShortExtra("Port", (short)2000);

		WiflyControl ctrl = new WiflyControl(ip, port);
		boolean done = ctrl.fwSetColor(0xffffffff, 0xff000000);
		Toast.makeText(getApplicationContext(), String.valueOf(done), Toast.LENGTH_SHORT).show();
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.activity_wifly_control, menu);
		return true;
	}

}
