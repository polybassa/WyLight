package biz.bruenn.WyLight;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

import biz.bruenn.WiflyLight.R;
import biz.bruenn.WyLight.ControlFragment.WiflyControlProvider;
import biz.bruenn.WyLight.exception.FatalError;
import android.content.Intent;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentActivity;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentPagerAdapter;
import android.support.v4.view.ViewPager;
import android.view.View;
import android.widget.Toast;

public class WiflyControlActivity extends FragmentActivity implements WiflyControlProvider {
	public static final String EXTRA_ENDPOINT = "biz.bruenn.WiflyLight.Endpoint";
	public static final String EXTRA_IP = "IpAddress";
	public static final String EXTRA_PORT = "Port";
	public static final short DEFAULT_PORT = 2000;
	
	private final WiflyControl mCtrl = new WiflyControl();
	private Endpoint mRemote;
	
	private class WiflyPagerAdapter extends FragmentPagerAdapter {

		public WiflyPagerAdapter(FragmentManager fm) {
			super(fm);
		}

		@Override
		public int getCount() {
			return 5;
		}

		@Override
		public Fragment getItem(int arg0) {
			switch(arg0) {
			case 2:
				ControlFragment brightness = new SetBrightnessFragment();
				return brightness;
			case 3:
				ControlFragment rgb = new SetRGBFragment();
				return rgb;
			case 1:
				ControlFragment color = new SetColorFragment();
				return color;
			case 4:
				ControlFragment gradient = new SetGradientFragment();
				return gradient;
			case 0:
				ControlFragment scripting = new ScriptingFragment();
				return scripting;
			default:
				ControlFragment setFade = new SetFadeFragment();
				return setFade;
			}
		}	
	}

	private String copyAssetToFile(String name) throws IOException {
		byte[] firmwareBuffer = new byte[1024];
		String outputFilename = getFilesDir().getAbsolutePath() + '/' + name;
		FileOutputStream out;
		out = new FileOutputStream(new File(outputFilename));
		InputStream firmwareAsset = this.getAssets().open(name);
		for(int bytesRead = firmwareAsset.read(firmwareBuffer); bytesRead > 0; bytesRead = firmwareAsset.read(firmwareBuffer)) {
			System.out.print(firmwareBuffer);
			out.write(firmwareBuffer);
		}
		firmwareAsset.close();
		out.close();
		return outputFilename;
	}

	public WiflyControl getControl() {
		return mCtrl;
	}
	
	public void onClickHandler(View view) {
		boolean done = false;
		switch(view.getId()) {
		case R.id.setClear:
			done = mCtrl.fwClearScript();
			break;
		case R.id.setLoopOff:
			done = mCtrl.fwLoopOff((byte) 0);
			break;
		case R.id.setLoopOn:
			done = mCtrl.fwLoopOn();
			break;
		}
		Toast.makeText(view.getContext(), String.valueOf(done), Toast.LENGTH_SHORT).show();
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		setContentView(R.layout.view_pager);
		
		ViewPager pager = (ViewPager)findViewById(R.id.pager);
		pager.setAdapter(new WiflyPagerAdapter(getSupportFragmentManager()));
		
		Intent i = getIntent();
		mRemote = (Endpoint) i.getSerializableExtra(EXTRA_ENDPOINT);
	}
	
	@Override
	protected void onPause() {
		mCtrl.disconnect();
		super.onPause();
	}
	
	@Override
	protected void onResume() {
		super.onResume();
		try {
			final String tempFilename = copyAssetToFile("main.hex");
			mCtrl.startup(mRemote, tempFilename);
		} catch (FatalError e) {
			Toast.makeText(getApplicationContext(), e.getMessage(), Toast.LENGTH_SHORT).show();
		} catch (IOException e) {
			Toast.makeText(getApplicationContext(), e.getMessage(), Toast.LENGTH_SHORT).show();
		}
	}
}
