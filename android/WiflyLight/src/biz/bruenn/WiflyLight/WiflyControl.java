package biz.bruenn.WiflyLight;

public class WiflyControl {
	private native long create(int ipv4Addr, short port);
	private native String ConfGetSsid(long pNative);
	private native boolean ConfSetWlan(long pNative, String passphrase, String ssid);
	private native boolean FwSetColor(long pNative, int addr, int rgba);
	private native void release(long pNative);
	
	private long mNative;
	
	public void finalize() throws Throwable {
		disconnect();
	}
	
	public boolean connect(int ipv4Addr, short port) {
		mNative = create(ipv4Addr, port);
		return 0 != mNative;
	}
	
	public void disconnect() {
		release(mNative);
		mNative = 0;
	}
	
	public String confGetSsid() {
		return ConfGetSsid(mNative);
	}
	
	public boolean confSetWlan(String passphrase, String ssid) {
		return ConfSetWlan(mNative, passphrase, ssid);
	}
	
	public boolean fwSetColor(int addr, int rgba) {
		return FwSetColor(mNative, addr, rgba);
	}
}
