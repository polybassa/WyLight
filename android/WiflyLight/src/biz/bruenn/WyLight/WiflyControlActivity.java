package biz.bruenn.WyLight;

import java.io.IOException;
import java.io.InputStream;
import java.util.HashSet;

import biz.bruenn.WiflyLight.R;
import biz.bruenn.WyLight.ControlFragment.WiflyControlProvider;
import biz.bruenn.WyLight.library.Endpoint;
import android.app.ActionBar;
import android.app.ActionBar.Tab;
import android.app.Activity;
import android.app.Fragment;
import android.app.FragmentManager;
import android.app.FragmentTransaction;
import android.content.Intent;
import android.os.Bundle;
import android.support.v13.app.FragmentPagerAdapter;
import android.support.v4.view.ViewPager;
import android.view.View;
import android.widget.Toast;

public class WiflyControlActivity extends Activity implements WiflyControlProvider {
	public static final String EXTRA_ENDPOINT = "biz.bruenn.WiflyLight.Endpoint";
	public static final String EXTRA_IP = "IpAddress";
	public static final String EXTRA_PORT = "Port";
	public static final short DEFAULT_PORT = 2000;
	
	private static final ControlFragment[] mFragments = new ControlFragment[] {
		new ScriptingFragment(),
		new SetColorFragment(),
		new SetBrightnessFragment(),
		new SetRGBFragment(),
		new SetGradientFragment()
	};

	private final HashSet<OnColorChangeListener> mColorChangedListenerList = new HashSet<OnColorChangeListener>();
	private final WiflyControl mCtrl = new WiflyControl();
	private Endpoint mRemote;
	private int mColor = 0xffffffff;
	
	public static class TabListener implements ActionBar.TabListener {
		private final ViewPager mPager;

		public TabListener(ViewPager pager) {
			mPager = pager;
		}

		public void onTabReselected(Tab tab, FragmentTransaction ft) {
			/* not implemented */
		}

		public void onTabSelected(Tab tab, FragmentTransaction ft) {
			mPager.setCurrentItem(tab.getPosition());
		}

		public void onTabUnselected(Tab tab, FragmentTransaction ft) {
			/* not implemented */
		}
	}

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
			return mFragments[arg0];
		}
	}

	public void addOnColorChangedListener(OnColorChangeListener listener) {
		mColorChangedListenerList.add(listener);
		listener.onColorChanged(mColor);
	}

	private String copyAssetToFile(String name) throws IOException {
		InputStream firmwareAsset = this.getAssets().open(name);
		CopyHelper copyHelper = new CopyHelper(getFilesDir().getAbsolutePath());
		return copyHelper.copyStreamToFile(firmwareAsset, name);
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

		ActionBar actionBar = getActionBar();
		actionBar.setNavigationMode(ActionBar.NAVIGATION_MODE_TABS);
		actionBar.setDisplayShowTitleEnabled(false);

		final ViewPager pager = (ViewPager)findViewById(R.id.pager);
		final WiflyPagerAdapter adapter =new WiflyPagerAdapter(getFragmentManager());
		pager.setAdapter(adapter);
		pager.setOnPageChangeListener(new ViewPager.OnPageChangeListener() {
			public void onPageScrolled(int arg0, float arg1, int arg2) { /* not implemented */ }
			public void onPageScrollStateChanged(int arg0) { /* not implemented */ }
			public void onPageSelected(int arg0) {
				getActionBar().setSelectedNavigationItem(arg0);
			}
		});

		TabListener listener = new TabListener(pager);
		for(int i = 0; i < pager.getAdapter().getCount(); ++i) {
			Tab tab = actionBar.newTab()
							   .setIcon(mFragments[i].getIcon())
							   .setTabListener(listener);
			actionBar.addTab(tab);
		}
		
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
			StartupTask startup = new StartupTask(this, mRemote, tempFilename);
			startup.execute(mCtrl);
		} catch (IOException e) {
			Toast.makeText(getApplicationContext(), e.getMessage(), Toast.LENGTH_SHORT).show();
		}
	}

	public void removeOnColorChangedListener(OnColorChangeListener listener) {
		mColorChangedListenerList.remove(listener);
	}

	public void setColor(int color) {
		mColor = color;
		for(OnColorChangeListener listener : mColorChangedListenerList) {
			listener.onColorChanged(color);
		}
	}
}
