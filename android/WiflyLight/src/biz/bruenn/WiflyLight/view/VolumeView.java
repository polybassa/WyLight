package biz.bruenn.WiflyLight.view;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.CornerPathEffect;
import android.graphics.LinearGradient;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.Rect;
import android.graphics.Shader;
import android.graphics.drawable.ShapeDrawable;
import android.graphics.drawable.shapes.PathShape;
import android.graphics.drawable.shapes.RectShape;
import android.view.MotionEvent;
import android.view.View;



public class VolumeView extends View {
	private ShapeDrawable mBar;
	private ShapeDrawable mCover;
	private ShapeDrawable mFrame;
	private Paint mFramePaint;
	private Path mFramePath;
	private int mPercent = 0;

	public VolumeView(Context context) {
		super(context);

		mBar = new ShapeDrawable(new RectShape());
		mBar.getPaint().setColor(Color.RED);
		
		mCover = new ShapeDrawable(new RectShape());
		mCover.getPaint().setColor(Color.GREEN);
	}
	
	@Override
	protected void onDraw(Canvas canvas) {
		canvas.drawColor(Color.BLACK);
		mBar.draw(canvas);
		mCover.draw(canvas);
		//mFrame.draw(canvas);
		
		canvas.drawPath(mFramePath, mFramePaint);
	}
	
	@Override
	protected void onSizeChanged(int w, int h, int oldw, int oldh) {
		final int left = w / 4;
		final int top = h / 10;
		final int right = w - left;
		final int bottom = h - top;
		mBar.setBounds(left, top, right, bottom);
		Shader barShader = new LinearGradient(right - left, 0, right - left, h, Color.WHITE, Color.BLACK, Shader.TileMode.REPEAT);
		mBar.getPaint().setShader(barShader);
		
		mPercent = bottom / 2;
		mCover.setBounds(left, top, right, bottom);
		
		mFramePath = new Path();
		mFramePath.moveTo(left, top);
		mFramePath.lineTo(w / 2, bottom);
		mFramePath.lineTo(right, top);
		mFramePath.lineTo(right, bottom);
		mFramePath.lineTo(left, bottom);
		mFramePath.close();
		

		mFramePaint = new Paint();
		//mFramePaint = mFrame.getPaint();
		mFramePaint.setColor(Color.YELLOW);
		mFramePaint.setStyle(Paint.Style.FILL_AND_STROKE);
		mFramePaint.setAntiAlias(true);
		mFramePaint.setPathEffect(new CornerPathEffect(10));
		
		mFrame = new ShapeDrawable(new PathShape(mFramePath, w, h));
		mFrame.setBounds(left, top, right, bottom);
		
		
	}
	
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		//mPercent = (int)event.getY();
		
		final Rect r = mBar.getBounds();
		int newY = (int)event.getY();
		mPercent = Math.min(r.bottom, Math.max(r.top, newY)); 
		mCover.setBounds(r.left, r.top, r.right, mPercent);
		
		//this.invalidateDrawable(mCover);
		this.invalidate();
		return true;
	}
}
