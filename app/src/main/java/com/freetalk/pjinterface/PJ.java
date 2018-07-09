package com.freetalk.pjinterface;

import android.util.Log;

public class PJ {

    public static final String TAG = "VP_PJ";

    public static native String helloJNI();
    private static native int createSipEngineJNI();
    private static native int registerPJClassJNI();
    private static native int destroyEngineJNI();

    public static int createSipEngine() {
        Log.v(TAG, "createSipEngine");
        registerPJClass();
        int status = 0;
        status = createSipEngineJNI();
        return status;
    }

    public static int registerPJClass() {
        Log.v(TAG, "registerPJClass");
        int status = 0;
        status = registerPJClassJNI();
        return status;
    }

    public static int destroyEngine() {
        Log.v(TAG, "destroyEngine");
        int status = 0;
        status = destroyEngineJNI();
        return status;
    }
}
