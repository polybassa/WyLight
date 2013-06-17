package biz.bruenn.WyLight.view;

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
import android.util.AttributeSet;
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
	private boolean mVertical = false;
	private final int mColor;
	private OnVolumeChangedListener mOnVolumeChangedListener = null;

	public VolumeView(Context context, AttributeSet attrib) {
		super(context, attrib);

		mVertical = attrib.getAttributeBooleanValue("http://schemas.android.com/apk/res/android", "orientation", false);
		mColor = attrib.getAttributeIntValue(null, "color", Color.WHITE);
		
		mBar = new ShapeDrawable(new RectShape());
		mCover = new ShapeDrawable(new RectShape());
		mFramePath = new Path();
		mFramePaint = new Paint();
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
		final int padding = (mVertical ? h : w) / 8;
		final int innerLeft = padding;
		final int innerTop = padding;
		final int innerRight = w - innerLeft;
		final int innerBottom = h - innerTop;
		final int innerWidth = w - 2*padding;
		final int innerHeight = h - 2*padding;
		final int outerLeft = 0;
		final int outerTop = 0;
		final int outerRight = w;
		final int outerBottom = h;
		
		mBar.setBounds(innerLeft, innerTop, innerRight, innerBottom);
		mCover.setBounds(innerLeft, innerTop, innerRight, innerBottom);
		
		if(mVertical) {
			Shader barShader = new LinearGradient(0, innerBottom - innerTop, w, innerBottom - innerTop, Color.BLACK, mColor, Shader.TileMode.REPEAT);
			mBar.getPaint().setShader(barShader);
			
			Shader frameShader = new LinearGradient(0, 0, innerHeight, 0, Color.GRAY, Color.DKGRAY, Shader.TileMode.MIRROR);
			mFramePaint.setShader(frameShader);
			
			mFramePath.reset();
			mFramePath.moveTo(innerRight, h / 2 + padding);
			mFramePath.lineTo(outerRight, h / 2);
			mFramePath.lineTo(outerRight, outerTop);
			mFramePath.lineTo(outerLeft, outerTop);
			mFramePath.lineTo(outerLeft, outerBottom);
			mFramePath.lineTo(outerRight, outerBottom);
			mFramePath.lineTo(outerRight, h/2);
			mFramePath.lineTo(innerRight, h / 2 - padding);
			mFramePath.lineTo(innerRight, innerBottom);
			mFramePath.lineTo(innerLeft, h / 2 + padding);
			mFramePath.lineTo(innerLeft, h / 2 - padding);
			mFramePath.lineTo(innerRight, innerTop);
			mFramePath.close();
		} else {
			Shader barShader = new LinearGradient(innerRight - innerLeft, 0, innerRight - innerLeft, h, mColor, Color.BLACK, Shader.TileMode.REPEAT);
			mBar.getPaint().setShader(barShader);
			
			Shader frameShader = new LinearGradient(0, 0, innerWidth, 0, Color.GRAY, Color.DKGRAY, Shader.TileMode.MIRROR);
			mFramePaint.setShader(frameShader);
			
			mFramePath.reset();
			mFramePath.moveTo(w / 2 + padding, innerTop);
			mFramePath.lineTo(w / 2, outerTop);
			mFramePath.lineTo(outerLeft, outerTop);
			mFramePath.lineTo(outerLeft, outerBottom);
			mFramePath.lineTo(outerRight, outerBottom);
			mFramePath.lineTo(outerRight, outerTop);
			mFramePath.lineTo(w / 2, outerTop);
			mFramePath.lineTo(w / 2 - padding, innerTop);
			mFramePath.lineTo(innerRight, innerTop);
			mFramePath.lineTo(w / 2 + padding, innerBottom);
			mFramePath.lineTo(w / 2 - padding, innerBottom);
			mFramePath.lineTo(innerLeft, innerTop);
			mFramePath.close();
		}
	}
	
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		final Rect r = mBar.getBounds();
		final int newX = (int)event.getX();
		final int newY = (int)event.getY();
		final int coverBottom = Math.min(r.bottom, Math.max(r.top, newY));
		final int coverRight = Math.min(r.right, Math.max(r.left, newX));
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
				if(mVertical) {
					mOnVolumeChangedListener.onVolumeChanged(100 * (coverRight - r.left) / (r.right - r.left));
				} else {
					mOnVolumeChangedListener.onVolumeChanged(100 - 100 * (coverBottom - r.top) / (r.bottom - r.top));
				}
			}
		}

		// we moved the selection -> repaint
		if(mVertical) {
			mCover.setBounds(coverRight, r.top, r.right, r.bottom);
		} else {
			mCover.setBounds(r.left, r.top, r.right, coverBottom);
		}
		this.invalidate();
		return true;
	}
	
	public void setOnVolumeChangedListener(OnVolumeChangedListener listener) {
		mOnVolumeChangedListener  = listener;
	}
}
