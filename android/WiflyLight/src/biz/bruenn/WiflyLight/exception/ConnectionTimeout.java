package biz.bruenn.WiflyLight.exception;

public class ConnectionTimeout extends FatalError {

	private static final long serialVersionUID = 8118891369159666734L;

	ConnectionTimeout(String s) {
		super("ConnectionTimeout: " + s);
	}
}
