package biz.bruenn.WyLight.exception;

public class FatalError extends Exception {
	
	private static final long serialVersionUID = -8293607201037488831L;
	
	FatalError(String s) {
		super("FatalError: " + s);
	}
}
