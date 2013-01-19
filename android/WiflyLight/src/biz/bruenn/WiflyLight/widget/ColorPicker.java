package biz.bruenn.WiflyLight.widget;

import java.util.ArrayList;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.LinearGradient;
import android.graphics.Paint;
import android.graphics.Shader;
import android.graphics.drawable.ShapeDrawable;
import android.graphics.drawable.shapes.RectShape;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.View;

public class ColorPicker extends View {
	public interface OnColorChangeListener {
		public void onColorChange(int red);
	};

	private static final int SECTOR_NUM = 3; 
	private static final int SECTOR_WIDTH = 50; 
	
	private ShapeDrawable mDrawable;
	private ArrayList<LinearGradient> mGradient;
	private LinearGradient mGradient_2;
	private LinearGradient mGradient_3;
	private OnColorChangeListener mOnColorChangeListener;

	public ColorPicker(Context context, AttributeSet attrs) {
		super(context, attrs);
		
		mGradient = new ArrayList<LinearGradient>(SECTOR_NUM);
		int x = 0;
		int y = 0;
		int width = 200;
		int height = 200;
		mDrawable = new ShapeDrawable(new RectShape());
		mDrawable.getPaint().setARGB(0xff, 0xff, 0, 0);
		mDrawable.setBounds(x, y, x+width, y + height);
		

		mGradient.add(new LinearGradient(0, y, 50, y,
				new int[]{0xffff0000, 0xffff00ff}, null, Shader.TileMode.CLAMP));
		
		mGradient.add(new LinearGradient(50, y, 100, y,
				new int[]{0xffff00ff, 0xff00ffff}, null, Shader.TileMode.CLAMP));
		mGradient.add(new LinearGradient(100, y, 150, y,
				new int[]{0xff00ffff, 0xff00ff00}, null, Shader.TileMode.CLAMP));

		this.setOnTouchListener(new View.OnTouchListener() {
			//TODO make this static
			final int mSectorBasecolor[] = {
				0xffff0000,
				0xffff00ff, 
				0xff0000ff,
				0xff00ffff,
				0xff00ff00,
				0xffffff00};
			
			
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
	
	protected void onDraw(Canvas canvas) {
		Paint p = new Paint();
		
		p.setShader(mGradient.get(0));
		canvas.drawRect(0, 0, 50, 200, p);
		p.setShader(mGradient.get(1));
		canvas.drawRect(50, 0, 100, 200, p);
		p.setShader(mGradient.get(2));
		canvas.drawRect(100, 0, 150, 200, p);
	}
	
	public void setOnColorChangeListener(OnColorChangeListener l) {
		mOnColorChangeListener = l;
	}
}
