package biz.bruenn.WiflyLight;

import android.os.Bundle;
import android.app.Activity;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.drawable.BitmapDrawable;
import android.view.Menu;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.Toast;
import biz.bruenn.WiflyLight.R.id;

public class WiflyControlActivity extends Activity {
	public static final String EXTRA_IP = "IpAddress";
	public static final String EXTRA_PORT = "Port";
	public static final short DEFAULT_PORT = 2000;
	
	private WiflyControl mCtrl;
	private Button mSetColorBtn;
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
		
		ImageView colorPicker = (ImageView)findViewById(id.colorPicker);
		colorPicker.setOnTouchListener(new View.OnTouchListener() {	
			public boolean onTouch(View v, MotionEvent event) {
				Bitmap b = ((BitmapDrawable)((ImageView)v).getDrawable()).getBitmap();
				int x = Math.max(0, Math.min(b.getWidth()-1, (int)event.getX()));
				int y = Math.max(0, Math.min(b.getHeight()-1, (int)event.getY()));
				mColor = b.getPixel(x, y);
				mSetColorBtn.setBackgroundColor(mColor);
				mSetColorBtn.setText(Integer.toHexString(mColor));
				return true;
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
