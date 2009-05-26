package gobang;

public class JNIAdapter {
	public native void reset();

	public native int GenerateChessInfo();

	public native void playChess(int info);

	static {
		System.loadLibrary("JNIAdapter");
	}
}
