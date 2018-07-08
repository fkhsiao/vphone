#include <jni.h>

#define __PJAPI__

#include "jni-include/common.h"

pjsua_transport_id udp_id;
int engineInitialized;

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

    // Must create pjsua before anything else!
    status = pjsua_create();
    if (status != PJ_SUCCESS) {
        pjsua_perror("pjApi.c", "Error initializing pjsua", status);
        return status;
    }

    // Create a memory pool to be used by JNI services calling pj functions.
    poolJNI = pjsua_pool_create("vphonePool", 4096, 4096);
    if (poolJNI == NULL)
    {
        return status;    // error on creating our memory pool.
    }


    // Initialize configs with default settings.
    pjsua_config_default(&ua_cfg);
    pjsua_logging_config_default(&log_cfg);
    pjsua_media_config_default(&media_cfg);

    ua_cfg.thread_cnt = 1;   // number of worker threads defaults to 1
    ua_cfg.max_calls = maxCalls;

    ua_cfg.enable_unsolicited_mwi = 1;  // allow unsolicited on all accounts


    // Initialize pjsua
    status = pjsua_init(&ua_cfg, &log_cfg, &media_cfg);
    if (status != PJ_SUCCESS) {
        pjsua_perror("pjApi.c", "Error initializing pjsua", status);
        return status;
    }

    // Create transport mechanisms  -- UDP
    pjsua_transport_config_default(&transport_cfg);
    transport_cfg.port = 5070; //This is only temporary for apollo. Need to ensure to use 5060.


    status = pjsua_transport_create(PJSIP_TRANSPORT_UDP, &transport_cfg, &udp_id);
    if (status != PJ_SUCCESS) {
        pjsua_perror("pjApi.c", "Error creating UDP transport", status);
        return status;
    }

    status = pjsua_start();
    if (status != PJ_SUCCESS) {
        pjsua_destroy();
        pjsua_perror("pjApi.c", "Error starting pjsua", status);
        return status;
    }

    engineInitialized = 1;

    return status;
}

JNIEXPORT jint Java_com_freetalk_pjinterface_PJ_registerPJClassJNI (JNIEnv* env, jobject obj)
{
    jclass pjClass = (*env)->FindClass(env, "com/freetalk/pjinterface/PJ");
    g_obj = (*env)->NewGlobalRef(env, pjClass);
    __android_log_print(ANDROID_LOG_VERBOSE, TAG, "registerPJClassJNI");
    return 1;
}

JNIEXPORT jint Java_com_freetalk_pjinterface_PJ_destroyEngineJNI(JNIEnv* env, jobject obj)
{
    int status = 0;

    pj_pool_release(poolJNI);
    poolJNI = NULL;

    engineInitialized = 0;

    __android_log_print(ANDROID_LOG_INFO, TAG, "Destroy SIP Engine");
    status = pjsua_destroy();
    return status;
}

// returns zero if the engine has been initialized.  // Prints error message if not initialized
int checkEngine(char * pFcnName) {
    if (engineInitialized == 0) {
        if (pFcnName != NULL)
            __android_log_print(ANDROID_LOG_WARN, TAG, "CheckEngine: not initialized: %s", pFcnName);
        else {
            __android_log_print(ANDROID_LOG_WARN, TAG, "CheckEngine: not initialized");
        }
        return -1000;
    } else {
        return 0;
    }
}

#undef __PJAPI__




