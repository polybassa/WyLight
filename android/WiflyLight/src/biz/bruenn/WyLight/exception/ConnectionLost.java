package biz.bruenn.WyLight.exception;

public class ConnectionLost extends FatalError {

	private static final long serialVersionUID = -3727536382513084874L;

	ConnectionLost(String s) {
		super("ConnectionLost: " + s);
	}
}
