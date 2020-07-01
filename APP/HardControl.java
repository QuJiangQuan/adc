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