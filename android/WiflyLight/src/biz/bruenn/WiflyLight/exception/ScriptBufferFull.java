package biz.bruenn.WiflyLight.exception;

public class ScriptBufferFull extends FatalError {

	private static final long serialVersionUID = -2883857838061112606L;

	ScriptBufferFull(String s) {
		super("ScriptBufferFull: " + s);
	}

}
