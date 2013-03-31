package biz.bruenn.WiflyLight;

public class WiflyControl {
	
	public static final int ALL_LEDS = 0xffffffff;
	
	private native long create(int ipv4Addr, short port);
	private native String ConfGetSsid(long pNative);
	private native boolean ConfSetWlan(long pNative, String passphrase, String ssid);
	private native boolean FwClearScript(long pNative);
	private native boolean FwLoopOff(long pNative, byte numLoops);
	private native boolean FwLoopOn(long pNative);
	private native boolean FwSetColor(long pNative, int argb, int addr);
	private native boolean FwSetFade(long pNative, int argb, int addr, short fadeTime);
	private native void release(long pNative);
	
	private long mNative;
	
	public void finalize() throws Throwable {
		disconnect();
	}
	
	public synchronized boolean connect(Endpoint remote) {
		mNative = create(remote.getAddr(), remote.getPort());
		return 0 != mNative;
	}
	
	public synchronized void disconnect() {
		release(mNative);
		mNative = 0;
	}
	
	public synchronized String confGetSsid() {
		return ConfGetSsid(mNative);
	}
	
	public synchronized boolean confSetWlan(String passphrase, String ssid) {
		return ConfSetWlan(mNative, passphrase, ssid);
	}
	
	public synchronized boolean fwClearScript() {
		return FwClearScript(mNative);
	}
	
	public synchronized boolean fwLoopOff(byte numLoops) {
		return FwLoopOff(mNative, numLoops);
	}
	
	public synchronized boolean fwLoopOn() {
		return FwLoopOn(mNative);
	}
	
	public synchronized boolean fwSetColor(int argb, int addr) {
		return FwSetColor(mNative, argb, addr);
	}
	
	public synchronized boolean fwSetFade(int argb, int addr, short fadeTime) {
		return FwSetFade(mNative, argb, addr, fadeTime);
	}
}
