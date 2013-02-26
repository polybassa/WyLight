package biz.bruenn.WiflyLight;

public class Endpoint {
	private final int mAddr;
	private final short mPort;
	
	public Endpoint(int addr, short port) {
		mAddr = addr;
		mPort = port;
	}
	
	public Endpoint(long remote) {
		this((int)(remote >> 32), (short)(remote & 0x000000000000ffffL));
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
