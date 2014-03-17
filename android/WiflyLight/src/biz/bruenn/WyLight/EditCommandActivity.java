package biz.bruenn.WyLight;

import biz.bruenn.WyLight.R;
import biz.bruenn.WyLight.view.RgbVolumeView;
import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

public class EditCommandActivity extends Activity {
	
	private Button mSave = null;
	private int mColor;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_edit_command);
		
		mSave = (Button)this.findViewById(R.id.save);
		mSave.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
				setResult(mColor, getIntent());
				finish();
			}
		});
		
		RgbVolumeView rgb = (RgbVolumeView)this.findViewById(R.id.rgb_volume);
		rgb.setOnColorChangedListener(new RgbVolumeView.OnColorChangeListener() {
			public void onColorChanged(int color) {
				mSave.setBackgroundColor(color);
				mSave.setTextColor(0xff000000 | ~color);
				mColor = color;
			}
		});
    }
}
