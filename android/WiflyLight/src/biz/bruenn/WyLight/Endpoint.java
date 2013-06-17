package biz.bruenn.WyLight;

import java.io.Serializable;

import biz.bruenn.WyLight.exception.FatalError;

public class Endpoint implements Serializable {
	private static final long serialVersionUID = -8653676704955283379L;

	private native long connect(long pBroadcastReceiver, long fingerprint) throws FatalError;
	
	private final int mAddr;
	private final short mPort;
	private final long mParentBroadcastReceiver;
	
	public Endpoint(long parent, int addr, short port) {
		mAddr = addr;
		mPort = port;
		mParentBroadcastReceiver = parent;
	}
	
	public Endpoint(long parent, long fingerprint) {
		this(parent, (int)(fingerprint >> 32), (short)(fingerprint & 0x000000000000ffffL));
	}
	
	public long connect() throws FatalError {
		return connect(mParentBroadcastReceiver, getFingerprint());
	}

	public int getAddr() {
		return mAddr;
	}
	
	public long getFingerprint() {
		return (((long)mAddr) << 32) | ((long)mPort);
	}
	
	public short getPort() {
		return mPort;
	}
	
	public boolean isValid() {
		return (0 != mAddr) && (0 != mPort);
	}
	
	@Override
	public String toString() {
		int _4 = mAddr & 0xff;
		int _3 = Integer.rotateRight(mAddr, 8) & 0xff;
		int _2 = Integer.rotateRight(mAddr, 16) & 0xff;
		int _1 = Integer.rotateRight(mAddr, 24) & 0xff;
		return Integer.toString(_1) + '.' 
			 + Integer.toString(_2) + '.'
			 + Integer.toString(_3) + '.'
			 + Integer.toString(_4) + ':'
			 + String.valueOf(mPort);
	}
}
