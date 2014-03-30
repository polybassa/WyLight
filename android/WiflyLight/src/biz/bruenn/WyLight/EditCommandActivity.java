package biz.bruenn.WyLight;

import biz.bruenn.WyLight.R;
import biz.bruenn.WyLight.view.RgbVolumeView;
import android.app.Activity;
import android.graphics.Color;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

public class EditCommandActivity extends Activity {
	
	private int mColor;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_edit_command);

        mColor = getIntent().getIntExtra(EditScriptActivity.ITEM_COLOR, Color.BLACK);
		RgbVolumeView rgb = (RgbVolumeView)this.findViewById(R.id.rgb_volume);
		rgb.onColorChanged(null, mColor);
		rgb.setOnStatusClickListener(new View.OnClickListener() {
			public void onClick(View v) {
				setResult(mColor, getIntent());
				finish();
			}
		});
		rgb.setOnColorChangedListener(new RgbVolumeView.OnColorChangeListener() {
			public void onColorChanged(int color) {
				mColor = color;
			}
		});
    }
}
