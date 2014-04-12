package biz.bruenn.WyLight;

import biz.bruenn.WiflyLight.R;
import biz.bruenn.WyLight.view.RgbGradientView;
import biz.bruenn.WyLight.view.RgbVolumeView;
import android.app.Activity;
import android.graphics.Color;
import android.os.Bundle;
import android.view.View;
import android.widget.SeekBar;
import android.widget.TextView;

public class EditGradientActivity extends Activity {

	private int[] mColor;
	private TextView mTimeStatus;
	private SeekBar mTime;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_edit_gradient);

        mColor = getIntent().getIntArrayExtra(EditScriptActivity.ITEM_COLORS);
        if(null == mColor) {
            mColor = new int[]{Color.WHITE, Color.BLACK};
        }
		RgbGradientView rgb = (RgbGradientView)this.findViewById(R.id.rgb_volume);
		rgb.setColors(mColor);
		rgb.onColorChanged(null, mColor[0]);
		rgb.setOnStatusClickListener(new View.OnClickListener() {
			public void onClick(View v) {
				final int fadeTmms = FadeTime.indexToTmms(mTime.getProgress());
				getIntent().putExtra(EditScriptActivity.ITEM_TIME, fadeTmms);
				getIntent().putExtra(EditScriptActivity.ITEM_COLORS, mColor);
				setResult(mColor[0], getIntent());
				finish();
			}
		});
		rgb.setOnColorChangedListener(new RgbGradientView.OnColorChangeListener() {
			public void onColorChanged(int[] color) {
				mColor = color;
			}
		});
		mTimeStatus = (TextView)findViewById(R.id.timeStatus);
		mTime = (SeekBar)findViewById(R.id.timeVolume);
		mTime.setMax(FadeTime.getMaxIndex());
		mTime.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
			public void onStopTrackingTouch(SeekBar seekBar) {
				// TODO Auto-generated method stub
			}

			public void onStartTrackingTouch(SeekBar seekBar) {
				// TODO Auto-generated method stub
			}

			public void onProgressChanged(SeekBar seekBar, int progress,
					boolean fromUser) {
				updateTimeStatus(progress);
			}
		});
		final int time = getIntent().getIntExtra(EditScriptActivity.ITEM_TIME, 0);
		mTime.setProgress(FadeTime.timeToIndex(time));
		updateTimeStatus(mTime.getProgress());
    }

    private void updateTimeStatus(int duration) {
		mTimeStatus.setText("Duration: " + Integer.toString(duration) + " sec");
    }
}
