package biz.bruenn.WiflyLight;

public class WiflyControl {
	private native long create(int ipv4Addr, short port);
	private native boolean FwSetColor(long pNative, int addr, int rgba);
	private native void release(long pNative);
	
	private final long mNative;
	
	WiflyControl(int ipv4Addr, short port) {
		mNative = create(ipv4Addr, port);
	}
	
	public void finalize() throws Throwable {
		release(mNative);
	}
	
	boolean fwSetColor(int addr, int rgba) {
		return FwSetColor(mNative, addr, rgba);
	}
}
