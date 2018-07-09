#include <jni.h>

#define __PJAPI__

#include "jni-include/common.h"
#include <pjsua-lib/pjsua.h>

#ifndef TAG
#define TAG "pjApi.c"
#endif

#define SIP_DOMAIN "example.com"
#define SIP_USER "alice"
#define SIP_PASSWD "secret"

JNIEXPORT jstring Java_com_freetalk_pjinterface_PJ_helloJNI(JNIEnv* env, jobject obj)
{
    return (*env)->NewStringUTF(env,"Hello JNI");
}

JNIEXPORT jint Java_com_freetalk_pjinterface_PJ_createSipEngineJNI(JNIEnv* env, jobject obj)
{

    pjsua_acc_id acc_id;
    pj_status_t status;

/* Get JavaVM! */
    int jstatus = (*env)->GetJavaVM(env, &g_vm);
    if (jstatus != 0) {
        __android_log_print(ANDROID_LOG_INFO, TAG, "Failed to get pointer to Java Virtual Machine");
        return jstatus;
    }

/* Create pjsua first! */
    status = pjsua_create();
    if (status != PJ_SUCCESS) error_exit("Error in pjsua_create()", status);

    /* Validating SIP URL */
    status = pjsua_verify_url(SIP_DOMAIN);
    if (status != PJ_SUCCESS) error_exit("Invalid URL in argv", status);

/* Init pjsua */
    {
        pjsua_config cfg;
        pjsua_logging_config log_cfg;
        pjsua_config_default(&cfg);
        cfg.cb.on_incoming_call = &on_incoming_call;
        cfg.cb.on_call_media_state = &on_call_media_state;
        cfg.cb.on_call_state = &on_call_state;
        pjsua_logging_config_default(&log_cfg);
        log_cfg.console_level = 4;
        status = pjsua_init(&cfg, &log_cfg, NULL);
        if (status != PJ_SUCCESS) error_exit("Error in pjsua_init()", status);
    }

/* Add UDP transport. */
    {
        pjsua_transport_config cfg;
        pjsua_transport_config_default(&cfg);
        cfg.port = 5060;
        status = pjsua_transport_create(PJSIP_TRANSPORT_UDP, &cfg, NULL);
        if (status != PJ_SUCCESS) error_exit("Error creating transport", status);
    }

/* Initialization is done, now start pjsua */
    status = pjsua_start();
    if (status != PJ_SUCCESS) error_exit("Error starting pjsua", status);

/* Register to SIP server by creating SIP account. */
    {
        pjsua_acc_config cfg;
        pjsua_acc_config_default(&cfg);
        cfg.id = pj_str("sip:" SIP_USER "@" SIP_DOMAIN);
        cfg.reg_uri = pj_str("sip:" SIP_DOMAIN);
        cfg.cred_count = 1;
        cfg.cred_info[0].realm = pj_str(SIP_DOMAIN);
        cfg.cred_info[0].scheme = pj_str("digest");
        cfg.cred_info[0].username = pj_str(SIP_USER);
        cfg.cred_info[0].data_type = PJSIP_CRED_DATA_PLAIN_PASSWD;
        cfg.cred_info[0].data = pj_str(SIP_PASSWD);
        status = pjsua_acc_add(&cfg, PJ_TRUE, &acc_id);
        if (status != PJ_SUCCESS) error_exit("Error adding account", status);
    }

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

    __android_log_print(ANDROID_LOG_INFO, TAG, "Destroy SIP Engine");
    status = pjsua_destroy();
    return status;
}

/* Callback called by the library upon receiving incoming call */
void on_incoming_call(pjsua_acc_id acc_id, pjsua_call_id call_id,
                             pjsip_rx_data *rdata)
{
    pjsua_call_info ci;

    PJ_UNUSED_ARG(acc_id);
    PJ_UNUSED_ARG(rdata);

    pjsua_call_get_info(call_id, &ci);

    PJ_LOG(3,(TAG, "Incoming call from %.*s!!",
            (int)ci.remote_info.slen,
            ci.remote_info.ptr));

    /* Automatically answer incoming calls with 200/OK */
    pjsua_call_answer(call_id, 200, NULL, NULL);
}

/* Callback called by the library when call's state has changed */
void on_call_state(pjsua_call_id call_id, pjsip_event *e)
{
    pjsua_call_info ci;

    PJ_UNUSED_ARG(e);

    pjsua_call_get_info(call_id, &ci);
    PJ_LOG(3,(TAG, "Call %d state=%.*s", call_id,
            (int)ci.state_text.slen,
            ci.state_text.ptr));
}

/* Callback called by the library when call's media state has changed */
void on_call_media_state(pjsua_call_id call_id)
{
    pjsua_call_info ci;

    pjsua_call_get_info(call_id, &ci);

    if (ci.media_status == PJSUA_CALL_MEDIA_ACTIVE) {
        // When media is active, connect call to sound device.
        pjsua_conf_connect(ci.conf_slot, 0);
        pjsua_conf_connect(0, ci.conf_slot);
    }
}

void error_exit(const char *title, pj_status_t status)
{
    pjsua_perror(TAG, title, status);
    pjsua_destroy();
    exit(1);
}

#undef __PJAPI__




