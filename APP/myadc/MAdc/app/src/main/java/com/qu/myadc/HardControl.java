package com.qu.myadc;

/**
 * Created by Administrator on 2020/6/28 0028.
 */
public class HardControl {
    public static native int  adcOpen();
    public static native String adcRead();
    static {
        try {
            System.loadLibrary("hardcontrol");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
