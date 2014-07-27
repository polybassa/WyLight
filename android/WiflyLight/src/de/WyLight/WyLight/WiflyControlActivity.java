package de.WyLight.WyLight;

import java.io.IOException;
import java.io.InputStream;
import java.util.HashSet;

import de.WyLight.WyLight.R;
import de.WyLight.WyLight.ControlFragment.WiflyControlProvider;
import de.WyLight.WyLight.exception.ConnectionTimeout;
import de.WyLight.WyLight.exception.FatalError;
import de.WyLight.WyLight.exception.ScriptBufferFull;
import de.WyLight.WyLight.library.Endpoint;
import de.WyLight.WyLight.library.ScriptAdapter;
import android.app.ActionBar;
import android.app.ActionBar.Tab;
import android.app.Activity;
import android.app.Fragment;
import android.app.FragmentManager;
import android.app.FragmentTransaction;
import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.support.v13.app.FragmentPagerAdapter;
import android.support.v4.view.ViewPager;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.widget.Toast;

public class WiflyControlActivity extends Activity implements
		WiflyControlProvider {
	private static final String STATE_KEY_COLOR = "mColor";
	public static final String EXTRA_ENDPOINT = "biz.bruenn.WiflyLight.Endpoint";
	public static final String EXTRA_IP = "IpAddress";
	public static final String EXTRA_PORT = "Port";
	public static final short DEFAULT_PORT = 2000;

	private static final ControlFragment[] mFragments = new ControlFragment[] {
			new SetColorFragment(), new SetBrightnessFragment(),
			new SetRGBFragment(), new SetScriptFragment(),
	// new SetGradientFragment()
	};

	private final HashSet<OnColorChangeListener> mColorChangedListenerList = new HashSet<OnColorChangeListener>();
	private final WiflyControl mCtrl = new WiflyControl();
	private Endpoint mRemote;
	private int mARGB = 0;
	private float[] mHSV = new float[] { 0f, 0f, 1f };
	private Menu mMenu;

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
			return mFragments.length;
		}

		@Override
		public Fragment getItem(int arg0) {
			return mFragments[arg0];
		}
	}

	public void addOnColorChangedListener(OnColorChangeListener listener) {
		mColorChangedListenerList.add(listener);
		listener.onColorChanged(mHSV, mARGB);
	}

	private String copyAssetToFile(String name) throws IOException {
		InputStream firmwareAsset = this.getAssets().open(name);
		CopyHelper copyHelper = new CopyHelper(getFilesDir().getAbsolutePath());
		return copyHelper.copyStreamToFile(firmwareAsset, name);
	}

	public WiflyControl getControl() {
		return mCtrl;
	}

	private void onColorChanged() {
		for (OnColorChangeListener listener : mColorChangedListenerList) {
			listener.onColorChanged(mHSV, mARGB);
		}
		try {
			mCtrl.fwSetColor(Color.HSVToColor(mHSV), WiflyControl.ALL_LEDS);
		} catch (ConnectionTimeout e) {
			onConnectionLost();
		} catch (ScriptBufferFull e) {
			onScriptBufferFull();
		} catch (FatalError e) {
			onFatalError(e);
		}
	}

	private void onConnectionLost() {
		Toast.makeText(this, "Connection lost", Toast.LENGTH_SHORT).show();
		finish();
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		if (null != savedInstanceState) {
			mARGB = savedInstanceState.getInt(STATE_KEY_COLOR, 0xffffffff);
			Color.colorToHSV(mARGB, mHSV);
		}

		setContentView(R.layout.view_pager);

		ActionBar actionBar = getActionBar();
		actionBar.setNavigationMode(ActionBar.NAVIGATION_MODE_TABS);
		actionBar.setDisplayShowTitleEnabled(false);

		final ViewPager pager = (ViewPager) findViewById(R.id.pager);
		final WiflyPagerAdapter adapter = new WiflyPagerAdapter(
				getFragmentManager());
		pager.setAdapter(adapter);
		pager.setOnPageChangeListener(new ViewPager.OnPageChangeListener() {
			public void onPageScrolled(int arg0, float arg1, int arg2) { /*
																		 * not
																		 * implemented
																		 */
			}

			public void onPageScrollStateChanged(int arg0) { /* not implemented */
			}

			public void onPageSelected(int arg0) {
				getActionBar().setSelectedNavigationItem(arg0);
				mFragments[arg0].onShow(mMenu);
			}
		});

		TabListener listener = new TabListener(pager);
		for (int i = 0; i < pager.getAdapter().getCount(); ++i) {
			Tab tab = actionBar.newTab().setIcon(mFragments[i].getIcon())
					.setTabListener(listener);
			actionBar.addTab(tab);
		}

		Intent i = getIntent();
		mRemote = (Endpoint) i.getSerializableExtra(EXTRA_ENDPOINT);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		mMenu = menu;
		MenuInflater inflater = getMenuInflater();
		inflater.inflate(R.menu.control_activity_actions, menu);
		return super.onCreateOptionsMenu(menu);
	}

	private void onFatalError(FatalError e) {
		Toast.makeText(this, e.getMessage(), Toast.LENGTH_SHORT).show();
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		switch (item.getItemId()) {
		case R.id.action_stop:
			mCtrl.fwClearScript();
			return true;
		default:
			return super.onOptionsItemSelected(item);
		}
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
			Toast.makeText(getApplicationContext(),
					"Firmware '" + e.getMessage() + "' is missing",
					Toast.LENGTH_SHORT).show();
			finish();
		}
	}

	@Override
	protected void onSaveInstanceState(Bundle outState) {
		super.onSaveInstanceState(outState);
		outState.putInt(STATE_KEY_COLOR, mARGB);
	}

	private void onScriptBufferFull() {
		Toast.makeText(this, R.string.msg_scriptbufferfull, Toast.LENGTH_LONG)
				.show();
	}

	public void removeOnColorChangedListener(OnColorChangeListener listener) {
		mColorChangedListenerList.remove(listener);
	}

	public void sendScript(ScriptAdapter script) {
		try {
			mCtrl.fwClearScript();
			mCtrl.fwSendScript(script);
		} catch (ConnectionTimeout e) {
			onConnectionLost();
		} catch (ScriptBufferFull e) {
			onScriptBufferFull();
		} catch (FatalError e) {
			onFatalError(e);
		}
	}

	public void setColor(int argb) {
		mARGB = argb;
		Color.colorToHSV(argb, mHSV);
		onColorChanged();
	}

	public void setColorHueSaturation(float hue, float saturation) {
		mHSV[0] = hue;
		mHSV[1] = saturation;
		mARGB = Color.HSVToColor(mHSV);
		onColorChanged();
	}

	public void setColorValue(float value) {
		mHSV[2] = value;
		mARGB = Color.HSVToColor(mHSV);
		onColorChanged();
	}
}
