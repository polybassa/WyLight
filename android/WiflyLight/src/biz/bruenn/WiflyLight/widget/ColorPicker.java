package biz.bruenn.WiflyLight.widget;

import java.util.ArrayList;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.LinearGradient;
import android.graphics.Paint;
import android.graphics.Rect;
import android.graphics.Shader;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.View;

public class ColorPicker extends View {
	public interface OnColorChangeListener {
		public void onColorChange(int red);
	};

	private static final int SECTOR_NUM = 6;
	private static final int mSectorBasecolor[] = {
			0xffff0000,
			0xffff00ff, 
			0xff0000ff,
			0xff00ffff,
			0xff00ff00,
			0xffffff00,
			0xffff0000};
	
	private ArrayList<LinearGradient> mGradient;
	private Paint mPaint = new Paint();
	private ArrayList<Rect> mRect;
	private OnColorChangeListener mOnColorChangeListener;

	public ColorPicker(Context context, AttributeSet attrs) {
		super(context, attrs);

		this.setOnTouchListener(new View.OnTouchListener() {			
			public boolean onTouch(View v, MotionEvent event) {
				final int sectorWidth = (v.getWidth() / 6) + 1;
				final int pixelX = (int)event.getX();
				final int sector = pixelX / sectorWidth;
				int sectorOffset = pixelX % sectorWidth;
				int color;
				if(sector < 0 || sector >= 6) {
					return false;
				}
				final int shift[] = {0, 16, 8};
				
				if(0 == sector % 2) {
					color = mSectorBasecolor[sector] + (sectorOffset << shift[sector % 3]);
				} else {
					color = mSectorBasecolor[sector] - (sectorOffset << shift[sector % 3]);
				}
				float y = event.getY();
				mOnColorChangeListener.onColorChange(color);
				return true;
			}
		});
	}
	
	@Override
	protected void onSizeChanged(int w, int h, int oldw, int oldh) {
		mGradient = new ArrayList<LinearGradient>(SECTOR_NUM);
		mRect = new ArrayList<Rect>(SECTOR_NUM);

		final int sectorWidth = w / SECTOR_NUM;
		for(int sector = 0; sector < SECTOR_NUM; sector++) {
			final int x = sector*sectorWidth;
			mGradient.add(new LinearGradient(x, 0, x + sectorWidth, 0,
					new int[]{mSectorBasecolor[sector], mSectorBasecolor[sector+1]}, null, Shader.TileMode.CLAMP));
			mRect.add(new Rect(x, 0, x + sectorWidth, h));
		}	
	}
	
	protected void onDraw(Canvas canvas) {
		int index = 0;
		for(LinearGradient lg: mGradient) {
			mPaint.setShader(lg);
			canvas.drawRect(mRect.get(index), mPaint);
			index++;
		}		
	}
	
	public void setOnColorChangeListener(OnColorChangeListener l) {
		mOnColorChangeListener = l;
	}
}

