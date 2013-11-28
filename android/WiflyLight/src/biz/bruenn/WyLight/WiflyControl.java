package biz.bruenn.WyLight;

import biz.bruenn.WyLight.exception.ConnectionTimeout;
import biz.bruenn.WyLight.exception.FatalError;
import biz.bruenn.WyLight.exception.ScriptBufferFull;
import biz.bruenn.WyLight.library.ScriptAdapter;

public class WiflyControl {
	
	public static final int ALL_LEDS = 0xffffffff;

	private native String ConfGetDeviceId(long pNative);
	private native String ConfGetPassphrase(long pNative);
	private native boolean ConfGetSoftAp(long pNative);
	private native String ConfGetSsid(long pNative);
	private native boolean ConfSetWlan(long pNative, String passphrase, String ssid, String deviceId, boolean softAp);
	private native boolean FwClearScript(long pNative);
	private native boolean FwLoopOff(long pNative, byte numLoops);
	private native boolean FwLoopOn(long pNative);
	private native boolean FwSendScript(long pNative, long pNativeScript) throws ConnectionTimeout, FatalError, ScriptBufferFull;
	private native boolean FwSetColor(long pNative, int argb, int addr) throws ConnectionTimeout, FatalError, ScriptBufferFull;
	private native boolean FwSetFade(long pNative, int argb, int addr, short fadeTime) throws ConnectionTimeout, FatalError, ScriptBufferFull;
	private native boolean FwSetGradient(long pNative, int argb_1, int argb_2, int length, int offset, short fadeTime) throws ConnectionTimeout, FatalError, ScriptBufferFull;
	private native void release(long pNative);
	private native void Startup(long pNative, String path);
	
	private long mNative;
	
	public void finalize() throws Throwable {
		disconnect();
	}
	
	public synchronized boolean connect(Endpoint remote) throws FatalError {
		mNative = remote.connect();
		return 0 != mNative;
	}
	
	public synchronized void disconnect() {
		release(mNative);
		mNative = 0;
	}
	
	public synchronized String confGetDeviceId() {
		return ConfGetDeviceId(mNative);
	}
	
	public synchronized String confGetPassphrase() {
		return ConfGetPassphrase(mNative);
	}
	
	public synchronized boolean confGetSoftAp() {
		return ConfGetSoftAp(mNative);
	}
	
	public synchronized String confGetSsid() {
		return ConfGetSsid(mNative);
	}
	
	public synchronized boolean confSetWlan(String passphrase, String ssid, String deviceId, boolean softAp) {
		return ConfSetWlan(mNative, passphrase, ssid, deviceId, softAp);
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
	
	public synchronized void fwSendScript(ScriptAdapter script) throws ConnectionTimeout, ScriptBufferFull, FatalError {
		FwSendScript(mNative, script.getNative());
	}
	
	public synchronized boolean fwSetColor(int argb, int addr) throws ConnectionTimeout, FatalError, ScriptBufferFull {
		return FwSetColor(mNative, argb, addr);
	}
	
	public synchronized boolean fwSetFade(int argb, int addr, short fadeTime) throws ConnectionTimeout, FatalError, ScriptBufferFull {
		return FwSetFade(mNative, argb, addr, fadeTime);
	}
	
	public synchronized boolean fwSetGradient(int argb_1, int argb_2, int length, int offset, short fadeTime) throws ConnectionTimeout, FatalError, ScriptBufferFull {
		return FwSetGradient(mNative, argb_1, argb_2, length, offset, fadeTime);
	}

	public synchronized void startup(Endpoint remote, String path) throws FatalError {
		if (connect(remote)) {
			Startup(mNative, path);
		}
	}
}
