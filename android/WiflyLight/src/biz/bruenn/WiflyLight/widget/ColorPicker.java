package biz.bruenn.WiflyLight.widget;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.drawable.ShapeDrawable;
import android.graphics.drawable.shapes.RectShape;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.View;

public class ColorPicker extends View {
	public interface OnColorChangeListener {
		public void onColorChange(int red);
	};
	
	private ShapeDrawable mDrawable;
	private OnColorChangeListener mOnColorChangeListener;

	public ColorPicker(Context context, AttributeSet attrs) {
		super(context, attrs);
		
		int x = 0;
		int y = 0;
		int width = 480;
		int height = 480;
		mDrawable = new ShapeDrawable(new RectShape());
		mDrawable.getPaint().setARGB(0xff, 0xff, 0, 0);
		mDrawable.setBounds(x, y, x+width, y + height);

		this.setOnTouchListener(new View.OnTouchListener() {
			
			public boolean onTouch(View v, MotionEvent event) {
				float x = (float)event.getX() / v.getWidth() * 255f;
				float y = event.getY();
				mOnColorChangeListener.onColorChange((int)x);
				return false;
			}
		});
	}
	
	protected void onDraw(Canvas canvas) {
		mDrawable.draw(canvas);
	}
	
	public void setOnColorChangeListener(OnColorChangeListener l) {
		mOnColorChangeListener = l;
	}
}
