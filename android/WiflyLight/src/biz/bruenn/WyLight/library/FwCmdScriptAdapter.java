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
		int result = getFadeColor(mNative);
		return result;
	}
	
	public void setColor(int argb) {
		setFadeColor(mNative, argb);
	}
}
