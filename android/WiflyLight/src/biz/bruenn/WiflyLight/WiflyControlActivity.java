package biz.bruenn.WiflyLight;

import biz.bruenn.WiflyLight.exception.FatalError;
import android.content.Intent;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentActivity;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentPagerAdapter;
import android.support.v4.view.ViewPager;
import android.view.View;
import android.widget.Toast;

public class WiflyControlActivity extends FragmentActivity {
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
			return 4;
		}

		@Override
		public Fragment getItem(int arg0) {
			switch(arg0) {
			case 0:
				ControlFragment brightness = new SetBrightnessFragment();
				brightness.setWiflyControl(mCtrl);
				return brightness;
			case 1:
				ControlFragment rgb = new SetRGBFragment();
				rgb.setWiflyControl(mCtrl);
				return rgb;
			case 2:
				ControlFragment color = new SetColorFragment();
				color.setWiflyControl(mCtrl);
				return color;
			default:
				ControlFragment setFade = new SetFadeFragment();
				setFade.setWiflyControl(mCtrl);
				return setFade;
			}
		}	
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
			mCtrl.connect(mRemote);
		} catch (FatalError e) {
			Toast.makeText(getApplicationContext(), e.getMessage(), Toast.LENGTH_SHORT).show();
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
