package biz.bruenn.WyLight.library;

public class FwCmdScriptAdapter {

	private native int getFadeColor(long pNative);
	private native int getFadeTime(long pNative);
	private native String getToken(long pNative);
	private native int setFadeColor(long pNative, int argb);
	private native int setFadeTime(long pNative, short time);
	
	private final long mNative;
	
	public FwCmdScriptAdapter(long pNative) {
		mNative = pNative;
	}
	
	public int getColor() {
		if(0 != mNative) {
			return getFadeColor(mNative);
		}
		return 0;
	}
	
	public int getTime() {
		if(0 != mNative) {
			return getFadeTime(mNative);
		}
		return 0;
	}

	public void setColor(int argb) {
		if(0 != mNative) {
			setFadeColor(mNative, argb);
		}
	}

	public void setTime(short time) {
		if(0 != mNative) {
			setFadeTime(mNative, time);
		}
	}
}
