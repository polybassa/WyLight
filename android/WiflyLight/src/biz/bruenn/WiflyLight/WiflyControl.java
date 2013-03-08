package biz.bruenn.WiflyLight;

public class WiflyControl {
	
	public static final int ALL_LEDS = 0xffffffff;
	
	private native long create(int ipv4Addr, short port);
	private native String ConfGetSsid(long pNative);
	private native boolean ConfSetWlan(long pNative, String passphrase, String ssid);
	private native boolean FwSetColor(long pNative, int addr, int rgba);
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
	
	public synchronized boolean fwSetColor(int addr, int rgba) {
		return FwSetColor(mNative, addr, rgba);
	}
}
