package biz.bruenn.WiflyLight;

public class Endpoint {
	private final int mAddr;
	private final short mPort;
	
	public Endpoint(long remote) {
		mAddr = (int)(remote >> 32);
		mPort = (short)(remote & 0x000000000000ffffL);
	}
	
	public int getAddr() {
		return mAddr;
	}
	
	public short getPort() {
		return mPort;
	}
	
	public boolean isValid() {
		return (0 != mAddr) && (0 != mPort);
	}
	
	@Override
	public String toString() {
		return Integer.toHexString(mAddr) + ':' + String.valueOf(mPort);
	}
}
