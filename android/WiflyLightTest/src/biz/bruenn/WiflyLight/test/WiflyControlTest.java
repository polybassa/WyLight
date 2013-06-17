package biz.bruenn.WiflyLight.test;

import biz.bruenn.WyLight.Endpoint;
import biz.bruenn.WyLight.WiflyControl;
import android.test.AndroidTestCase;

public class WiflyControlTest extends AndroidTestCase {
	private native long create(int ipv4Addr, short port);
	private native boolean ConfSetWlan(long pNative, String ssid, String passphrase);
	private native boolean FwSetColor(long pNative, int addr, int rgba);
	private native void release(long pNative);
	
	static {
		System.loadLibrary("wifly");
	}
	
	public WiflyControlTest() {
		super();
	}
	
	@Override
	protected void setUp() throws Exception {
		super.setUp();
	}
	
	@Override
	protected void tearDown() throws Exception {
		super.tearDown();
	}
	
	public void testMultipleInstances() {
		WiflyControl testeeOne = new WiflyControl();
		WiflyControl testeeTwo = new WiflyControl();
		Endpoint e = new Endpoint(0, (short)0);
		assertTrue(testeeOne.connect(e));
		assertFalse(testeeTwo.connect(e));		
	}
	
	public void testConfSetWlan() {
		WiflyControl testee = new WiflyControl();
		Endpoint e = new Endpoint(0, (short)0);
		String specialChars = new String();
		
		for(char c = 32; c < 127; c++) {
			specialChars += c;
		}
		
		assertTrue(testee.connect(e));
		assertTrue(testee.confSetWlan("passphrase", specialChars));
		String ssid = testee.confGetSsid();
		assertEquals(ssid, specialChars);
		testee.disconnect();
	}
}
