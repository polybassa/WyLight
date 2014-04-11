package biz.bruenn.WyLight.library;

public class FwCmdScriptAdapter {

	private native int getFadeColor(long pNative);
	private native int getFadeTime(long pNative);
	private native long getGradientColors(long pNative);
	private native String getToken(long pNative);
	private native char getType(long pNative);
	private native int setFadeColor(long pNative, int argb);
	private native int setFadeTime(long pNative, short time);
	
	private final long mNative;
	
	public enum Type {
		UNKOWN,
		FADE,
		GRADIENT
	}

	public FwCmdScriptAdapter(long pNative) {
		mNative = pNative;
	}
	
	public int getColor() {
		if(0 != mNative) {
			return getFadeColor(mNative);
		}
		return 0;
	}

	public int[] getGradientColor() {
		int[] temp = new int[2];
		if(0 != mNative) {
			long dual = getGradientColors(mNative);
			temp[0] = (int)(dual & 0xffffffff);
			temp[1] = (int)(dual >> 32);
		}
		return temp;
	}

	public int getTime() {
		if(0 != mNative) {
			return getFadeTime(mNative);
		}
		return 0;
	}

	public Type getType() {
		if(0 != mNative) {
			final char type = getType(mNative);
			if(0xF9 == type) {
				return Type.GRADIENT;
			} else if (0xFC == type) {
				return Type.FADE;
			}
		}
		return Type.UNKOWN;
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
