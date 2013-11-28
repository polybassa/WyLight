package biz.bruenn.WyLight.exception;

public class InvalidParameter extends FatalError {

	private static final long serialVersionUID = 7810409241984695478L;

	InvalidParameter(String s) {
		super("InvalidParameter: " + s);
	}
}
