package biz.bruenn.WiflyLight;

import android.app.Activity;
import android.os.Bundle;

public class WiflyLightActivity extends Activity {
	private native void runClient();
	
	static {
		System.loadLibrary("wifly");
	}
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        runClient();
    }
}