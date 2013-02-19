package biz.bruenn.WiflyLight;

import android.app.Activity;
import android.os.Bundle;

public class WiflyConfigActivity extends Activity {
	public static final String EXTRA_IP = "IpAddress";
	public static final String EXTRA_PORT = "Port";
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_wifly_config);
	}

}
