package biz.bruenn.WyLight;

import java.io.Serializable;

import biz.bruenn.WyLight.exception.ConnectionTimeout;
import biz.bruenn.WyLight.exception.FatalError;
import biz.bruenn.WyLight.exception.ScriptBufferFull;

public class ScriptCommand implements Serializable {
	private static final long serialVersionUID = 3259077254858405352L;
	
	private String mString;
	private int mColor = 0xff000000;
	public ScriptCommand(String temp) {
		mString = temp;
	}
	
	public int getColor() {
		return mColor;
	}
	
	public void sendTo(WiflyControl ctrl) throws ConnectionTimeout, ScriptBufferFull, FatalError {
		ctrl.fwSetFade(mColor, 0xffffffff, (short) 500);
	}
	
	public void setColor(int color) {
		mColor = color;
	}
	
	@Override
	public String toString() {
		return "S: " + mString + '(' + String.valueOf(mColor) + ')';
	}
}
