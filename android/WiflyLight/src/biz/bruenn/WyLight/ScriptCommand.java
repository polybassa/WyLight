package biz.bruenn.WyLight;

import java.io.Serializable;

public class ScriptCommand implements Serializable {
	private static final long serialVersionUID = 3259077254858405352L;
	
	private String mString;
	public ScriptCommand(String temp) {
		mString = temp;
	}
	
	@Override
	public String toString() {
		return "S: " + mString;
	}
}
