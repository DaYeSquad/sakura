package com.sakura.test;

/**
 * Created by FrankLin on 12/24/15.
 */
public class FeedMessageManager {

    public FeedMessageManager() {
        mNativeHandler = nativeCreateInstance();
    }

    public void connect(String url) {
        nativeConnect(mNativeHandler, url);
    }

    private native long nativeCreateInstance();

    private native void nativeConnect(long handler, String url);

    private volatile long mNativeHandler;
}
