package biz.bruenn.WiflyLight;

import android.os.Bundle;
import android.app.Activity;
import android.content.Intent;
import android.graphics.Color;
import android.view.Menu;
import android.view.View;
import android.widget.Button;
import android.widget.SeekBar;
import android.widget.Toast;
import biz.bruenn.WiflyLight.R.id;
import biz.bruenn.WiflyLight.widget.ColorPicker;

public class WiflyControlActivity extends Activity {
	public static final String EXTRA_IP = "IpAddress";
	public static final String EXTRA_PORT = "Port";
	public static final short DEFAULT_PORT = 2000;
	
	private WiflyControl mCtrl;
	private Button mSetColorBtn;
	private SeekBar mRed;
	private SeekBar mGreen;
	private SeekBar mBlue;
	private int mColor = 0xff000000;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_wifly_control);
		Intent i = getIntent();
		int ip = i.getIntExtra(EXTRA_IP, 0);
		short port = i.getShortExtra(EXTRA_PORT, DEFAULT_PORT);
		mCtrl = new WiflyControl(ip, port);
		
		mSetColorBtn = (Button)findViewById(R.id.setColor);
		mSetColorBtn.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
				int c = mColor << 8;
				boolean done = mCtrl.fwSetColor(0xffffffff, c);
				Toast.makeText(getApplicationContext(), String.valueOf(done), Toast.LENGTH_SHORT).show();
			}
		});
		

		mRed = (SeekBar)findViewById(R.id.red);
		mRed.setMax(255);
		mRed.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
			public void onStopTrackingTouch(SeekBar seekBar) {
			}
			public void onStartTrackingTouch(SeekBar seekBar) {	
			}
			public void onProgressChanged(SeekBar seekBar, int progress,
					boolean fromUser) {
				mColor = (mColor & 0xff00ffff) | (progress << 16);
				mSetColorBtn.setBackgroundColor(mColor);
			}
		});
		mGreen = (SeekBar)findViewById(R.id.green);
		mGreen.setMax(255);
		mGreen.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
			public void onStopTrackingTouch(SeekBar seekBar) {
			}
			public void onStartTrackingTouch(SeekBar seekBar) {	
			}
			public void onProgressChanged(SeekBar seekBar, int progress,
					boolean fromUser) {
				mColor = (mColor & 0xffff00ff) | (progress << 8);
				mSetColorBtn.setBackgroundColor(mColor);
			}
		});
		mBlue = (SeekBar)findViewById(R.id.blue);
		mBlue.setMax(255);
		mBlue.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
			public void onStopTrackingTouch(SeekBar seekBar) {
			}
			public void onStartTrackingTouch(SeekBar seekBar) {	
			}
			public void onProgressChanged(SeekBar seekBar, int progress,
					boolean fromUser) {
				mColor = (mColor & 0xffffff00) | (progress);
				mSetColorBtn.setBackgroundColor(mColor);
			}
		});
		
		ColorPicker colorPicker = (ColorPicker)findViewById(id.colorPicker);
		colorPicker.setOnColorChangeListener(new ColorPicker.OnColorChangeListener() {
			
			public void onColorChange(int red) {
				mColor = Color.rgb(red, 0, 0);
				mSetColorBtn.setBackgroundColor(mColor);
				mSetColorBtn.setText(String.valueOf(red));
			}
		});
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.activity_wifly_control, menu);
		return true;
	}
	
	@Override
	protected void onDestroy() {
		mCtrl = null;
		super.onDestroy();
	}
}
