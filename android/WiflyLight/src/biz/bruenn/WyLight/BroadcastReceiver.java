package biz.bruenn.WyLight;

public class BroadcastReceiver {
	private native long create(String path);
	private native long getEndpoint(long pNative, long index);
	private native long getNextRemote(long pNative, long timeoutNanos);
	private native void release(long pNative);
	
	private long mNative = 0;
	
	public BroadcastReceiver(String recentFilename) {
		mNative = create(recentFilename);
	}
	
	public Endpoint getEndpoint(long index) {
		return new Endpoint(mNative, getEndpoint(mNative, index));
	}
	
	public final long getNative() {
		return mNative;
	}
	
	public Endpoint getNextRemote(long timeoutNanos) {
		return new Endpoint(mNative, getNextRemote(mNative, timeoutNanos));
	}
	
	public void release() {
		release(mNative);
	}
}
