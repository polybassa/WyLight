package biz.bruenn.WyLight;

import java.io.Serializable;

import biz.bruenn.WyLight.exception.FatalError;

public class Endpoint implements Serializable {
	private static final long serialVersionUID = -8653676704955283379L;

	private native long connect(long pBroadcastReceiver, long fingerprint) throws FatalError;
	private native String getEndpointName(long pBroadcastReceiver, long fingerprint) throws FatalError;
	private native void setEndpointName(long pBroadcastReceiver, long fingerprint, String deviceId) throws FatalError;
	
	private final int mAddr;
	private final short mPort;
	private final long mParentBroadcastReceiver;
	private boolean mIsOnline = false;
	
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
	
	@Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		Endpoint other = (Endpoint) obj;
		if (mAddr != other.mAddr)
			return false;
		if (mParentBroadcastReceiver != other.mParentBroadcastReceiver)
			return false;
		if (mPort != other.mPort)
			return false;
		return true;
	}

	public int getAddr() {
		return mAddr;
	}
	
	public long getFingerprint() {
		return (((long)mAddr) << 32) | ((long)mPort);
	}
	
	public String getName() {
		try {
			return getEndpointName(mParentBroadcastReceiver, getFingerprint());
		} catch (FatalError e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		// use ip + port as a default name
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
	
	public short getPort() {
		return mPort;
	}
	
	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + mAddr;
		result = prime
				* result
				+ (int) (mParentBroadcastReceiver ^ (mParentBroadcastReceiver >>> 32));
		result = prime * result + mPort;
		return result;
	}
	
	public boolean isOnline() {
		return mIsOnline;
	}
	
	public boolean isValid() {
		return (0 != mAddr) && (0 != mPort);
	}
	
	public void setName(String deviceId) throws FatalError {
		setEndpointName(mParentBroadcastReceiver, getFingerprint(), deviceId);
	}
	
	public void setOnline(boolean isOnline) {
		mIsOnline = isOnline;
	}
	
	@Override
	public String toString() {
		/**
			 */
		return getName();
	}
}
