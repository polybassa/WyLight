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

	private LinearGradient mGradient;
	private Paint mPaint = new Paint();
	private Rect mRect;
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
		mSectorWidth = (w / SECTOR_NUM) + 1;
		mGradient = new LinearGradient(0, 0, w, 0, SECTOR_BASECOLOR, null, Shader.TileMode.CLAMP);
		mRect = new Rect(0, 0, w, h);
	}
	
	protected void onDraw(Canvas canvas) {
			mPaint.setShader(mGradient);
			canvas.drawRect(mRect, mPaint);
	}
	
	public void setOnColorChangeListener(OnColorChangeListener l) {
		mOnColorChangeListener = l;
	}
}

