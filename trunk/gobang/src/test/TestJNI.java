package test;

public class TestJNI {

	public static void main(String[] args) {
		System.loadLibrary("HelloNative");
		HelloNative.greeting();
	}
}
