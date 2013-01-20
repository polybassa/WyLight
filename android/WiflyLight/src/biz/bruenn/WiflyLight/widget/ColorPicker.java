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
	private static final int SECTOR_SHIFT[] = {0, 16, 8};
	private static final int SECTOR_BASECOLOR[] = {
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
	private int mSectorWidth;
	private OnColorChangeListener mOnColorChangeListener;

	public ColorPicker(Context context, AttributeSet attrs) {
		super(context, attrs);

		this.setOnTouchListener(new View.OnTouchListener() {			
			public boolean onTouch(View v, MotionEvent event) {
				final int pixelX = (int)event.getX();
				final int sector = pixelX / mSectorWidth;
				final int sectorOffset = (int)(pixelX % mSectorWidth * (255f/mSectorWidth));

				final int color;
				if(0 == sector % 2) {
					color = SECTOR_BASECOLOR[sector] + (sectorOffset << SECTOR_SHIFT[sector % 3]);
				} else {
					color = SECTOR_BASECOLOR[sector] - (sectorOffset << SECTOR_SHIFT[sector % 3]);
				}
				
				mOnColorChangeListener.onColorChange(color);
				return true;
			}
		});
	}
	
	@Override
	protected void onSizeChanged(int w, int h, int oldw, int oldh) {
		mGradient = new ArrayList<LinearGradient>(SECTOR_NUM);
		mRect = new ArrayList<Rect>(SECTOR_NUM);

		mSectorWidth = (w / SECTOR_NUM) + 1;
		for(int sector = 0; sector < SECTOR_NUM; sector++) {
			final int x = sector*mSectorWidth;
			mGradient.add(new LinearGradient(x, 0, x + mSectorWidth, 0,
					new int[]{SECTOR_BASECOLOR[sector], SECTOR_BASECOLOR[sector+1]}, null, Shader.TileMode.CLAMP));
			mRect.add(new Rect(x, 0, x + mSectorWidth, h));
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

