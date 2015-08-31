package com.android.gles3jni;

/**
 * Wrapper for native library
 */
public class PopupBookLib {
	static {
		System.loadLibrary("app");
	}

	public static native void init();
	public static native void resize(int width, int height);
	public static native void step();
}
