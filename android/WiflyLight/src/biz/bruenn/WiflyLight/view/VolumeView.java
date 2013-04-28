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
import android.graphics.drawable.shapes.RectShape;
import android.view.MotionEvent;
import android.view.View;



public class VolumeView extends View {
	
	public interface OnVolumeChangedListener {
		public void onVolumeChanged(int percent);
	}
	
	private ShapeDrawable mBar;
	private ShapeDrawable mCover;
	private Paint mFramePaint;
	private Path mFramePath;
	private boolean mEmbraceTouch = false;
	private OnVolumeChangedListener mOnVolumeChangedListener = null;

	public VolumeView(Context context) {
		super(context);

		mBar = new ShapeDrawable(new RectShape());
		
		mCover = new ShapeDrawable(new RectShape());
		//mCover.getPaint().setColor(Color.BLACK);
		
		mFramePath = new Path();
		mFramePaint = new Paint();
		mFramePaint.setColor(Color.YELLOW);
		mFramePaint.setStyle(Paint.Style.FILL_AND_STROKE);
		mFramePaint.setAntiAlias(true);
		mFramePaint.setPathEffect(new CornerPathEffect(10));
	}
	
	@Override
	protected void onDraw(Canvas canvas) {
		mBar.draw(canvas);
		mCover.draw(canvas);		
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
		
		mCover.setBounds(left, top, right, bottom);
		
		//Shader frameShader = new LinearGradient(0, 0, (right - left) / 4, 0, Color.BLACK, Color.DKGRAY, Shader.TileMode.MIRROR);
		//mFramePaint.setShader(frameShader);
		
		mFramePath.reset();
		mFramePath.moveTo(left, top);
		mFramePath.lineTo(w / 2, bottom);
		mFramePath.lineTo(right, top);
		mFramePath.lineTo(right, bottom);
		mFramePath.lineTo(left, bottom);
		mFramePath.close();
	}
	
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		final Rect r = mBar.getBounds();
		final int newX = (int)event.getX();
		final int newY = (int)event.getY();
		final int coverBottom = Math.min(r.bottom, Math.max(r.top, newY));
		final int action = event.getAction();
		
		// begin of touch event, check if touch was in range
		if(MotionEvent.ACTION_DOWN == action) {
			mEmbraceTouch = (r.left <= newX && newX <= r.right);
		}
		
		// we can ignore all further touch events
		if(!mEmbraceTouch) {
			return false;
		}
		
		// check for end of touch gesture
		if(MotionEvent.ACTION_UP == action) {
			mEmbraceTouch = false;
			if(null != mOnVolumeChangedListener) {
				mOnVolumeChangedListener.onVolumeChanged(100 - 100 * (coverBottom - r.top) / (r.bottom - r.top));
			}
		}

		// we moved the selection -> repaint
		mCover.setBounds(r.left, r.top, r.right, coverBottom);
		this.invalidate();
		return true;
	}
	
	public void setOnVolumeChangedListener(OnVolumeChangedListener listener) {
		mOnVolumeChangedListener  = listener;
	}
}
