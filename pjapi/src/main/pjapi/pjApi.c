#include <jni.h>

#define __PJAPI__

#include "jni-include/common.h"

JNIEXPORT jstring Java_com_freetalk_pjinterface_PJ_helloJNI(JNIEnv* env, jobject obj)
{
    return (*env)->NewStringUTF(env,"Hello JNI");
}

JNIEXPORT jint Java_com_freetalk_pjinterface_PJ_createSipEngineJNI(JNIEnv* env, jobject obj)
{
    pjsua_config ua_cfg;
    pjsua_logging_config log_cfg;
    pjsua_media_config media_cfg;
    pjsua_transport_config transport_cfg;
    pj_status_t status;
    
    int jstatus = (*env)->GetJavaVM(env, &g_vm);
    if (jstatus != 0) {
        __android_log_print(ANDROID_LOG_INFO, TAG, "Failed to get pointer to Java Virtual Machine");
        return jstatus;
    }

    return status;
}


#undef __PJAPI__




