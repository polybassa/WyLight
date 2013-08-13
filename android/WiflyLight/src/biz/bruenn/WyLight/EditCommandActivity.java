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
	public static final String EXTRA_COMMAND = "biz.bruenn.WyLight.ScriptCommand";
	
	private ScriptCommand mCmd;
	private Button mSave = null;
	private int mColor;
	

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_edit_command);

		Intent i = getIntent();
		mCmd = (ScriptCommand) i.getSerializableExtra(EXTRA_COMMAND);
		
		mSave = (Button)this.findViewById(R.id.save);
		mSave.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
				setResult(mColor);
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
