package com.freetalk.pjinterface;

public class PJ {
    public static native String helloJNI();
    public static native int createSipEngineJNI();
    public static native int registerPJClassJNI();
    public static native int destroyEngineJNI();
    public static int createSipEngine() {
        registerPJClass();
        int status = createSipEngineJNI();
        return status;
    }
    public static int registerPJClass() {
        int status;
        status = registerPJClassJNI();
        return status;
    }
    public static int destroyEngine() {
        int status = 0;
        status = destroyEngineJNI();
        return status;
    }
}
