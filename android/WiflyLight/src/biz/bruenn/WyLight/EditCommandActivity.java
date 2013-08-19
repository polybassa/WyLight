package biz.bruenn.WyLight;

import biz.bruenn.WiflyLight.R;
import biz.bruenn.WyLight.view.RgbVolumeView;
import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class EditCommandActivity extends Activity {
	
	private ScriptCommand mCmd;
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
		rgb.setOnColorChangedListener(new RgbVolumeView.OnColorChangedListener() {
			public void onColorChanged(int color) {
				mSave.setBackgroundColor(color);
				mSave.setTextColor(0xff000000 | ~color);
				mColor = color;
			}
		});
    }

}
