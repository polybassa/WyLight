package biz.bruenn.WiflyLight;

import android.content.Intent;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentActivity;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentPagerAdapter;
import android.support.v4.view.ViewPager;

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
			return 2;
		}

		@Override
		public Fragment getItem(int arg0) {
			switch(arg0) {
			case 0:
				ControlFragment f = new SetBrightnessFragment();
				f.setWiflyControl(mCtrl);
				return f;
			default:
				ControlFragment ff = new SetColorFragment();
				ff.setWiflyControl(mCtrl);
				return ff;
			}
		}
		
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
		mCtrl.connect(mRemote);
	}
}
