package biz.bruenn.WyLight.library;

public class FwCmdScriptAdapter {

	private native int getFadeColor(long pNative);
	private native String getToken(long pNative);
	private native int setFadeColor(long pNative, int argb);
	
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
	
	public void setColor(int argb) {
		if(0 != mNative) {
			setFadeColor(mNative, argb);
		}
	}
}
