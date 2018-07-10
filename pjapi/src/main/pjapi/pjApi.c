#include <jni.h>

#define __PJAPI__

#include "jni-include/common.h"
#include <pjsua-lib/pjsua.h>

#ifndef TAG
#define TAG "VP_pjApi.c"
#endif

#define SIP_DOMAIN "sip-25016.accounts.vocalocity.com"
#define SIP_USER "VH323744"
#define SIP_PASSWD "Vocal123"

JNIEXPORT jstring Java_com_freetalk_pjinterface_PJ_helloJNI(JNIEnv* env, jobject obj)
{
    return (*env)->NewStringUTF(env,"Hello JNI");
}

JNIEXPORT jint Java_com_freetalk_pjinterface_PJ_createSipEngineJNI(JNIEnv* env, jobject obj)
{
    __android_log_print(ANDROID_LOG_VERBOSE, TAG, "Java_com_freetalk_pjinterface_PJ_createSipEngineJNI");
    pjsua_acc_id acc_id;
    pj_status_t status;

/* Get JavaVM! */
    __android_log_print(ANDROID_LOG_VERBOSE, TAG, "Geting JavaVM");
    int jstatus = (*env)->GetJavaVM(env, &g_vm);
    if (jstatus != 0) {
        __android_log_print(ANDROID_LOG_VERBOSE, TAG, "Failed to get pointer to Java Virtual Machine");
        return jstatus;
    }
    __android_log_print(ANDROID_LOG_VERBOSE, TAG, "Getted pointer to Java Virtual Machine");

/* Create pjsua first! */
    __android_log_print(ANDROID_LOG_VERBOSE, TAG, "pjsua_creating()");
    status = pjsua_create();
    if (status != PJ_SUCCESS) error_exit("Error in pjsua_create()", status);
    __android_log_print(ANDROID_LOG_VERBOSE, TAG, "pjsua_created()");

//    /* Validating SIP URL */
    __android_log_print(ANDROID_LOG_VERBOSE, TAG, "Validing SIP_DOMAIN");
    status = pjsua_verify_url("sip:" SIP_USER "@" SIP_DOMAIN);
    if (status != PJ_SUCCESS) error_exit("Invalid URL", status);
    __android_log_print(ANDROID_LOG_VERBOSE, TAG, "Valided SIP_DOMAIN");

/* Init pjsua */
    {
        pjsua_config cfg;
        pjsua_logging_config log_cfg;
        pjsua_config_default(&cfg);
        cfg.cb.on_incoming_call = &on_incoming_call;
        cfg.cb.on_call_media_state = &on_call_media_state;
        cfg.cb.on_call_state = &on_call_state;
        cfg.user_agent = pj_str("vphone");

        pjsua_logging_config_default(&log_cfg);
        log_cfg.console_level = 4;
        __android_log_print(ANDROID_LOG_VERBOSE, TAG, "pjsua_initing()");
        status = pjsua_init(&cfg, &log_cfg, NULL);
        if (status != PJ_SUCCESS) error_exit("Error in pjsua_init()", status);
        __android_log_print(ANDROID_LOG_VERBOSE, TAG, "pjsua_initted()");
    }

/* Add UDP transport. */
    {
        pjsua_transport_config cfg;
        pjsua_transport_config_default(&cfg);
        cfg.port = 5060;
        __android_log_print(ANDROID_LOG_VERBOSE, TAG, "pjsua_creating transport");
        status = pjsua_transport_create(PJSIP_TRANSPORT_UDP, &cfg, NULL);
        if (status != PJ_SUCCESS) error_exit("Error creating transport", status);
    }

/* Initialization is done, now start pjsua */
    __android_log_print(ANDROID_LOG_VERBOSE, TAG, "pjsua_starting()");
    status = pjsua_start();
    if (status != PJ_SUCCESS) error_exit("Error starting pjsua", status);
    __android_log_print(ANDROID_LOG_VERBOSE, TAG, "pjsua_started()");

/* Register to SIP server by creating SIP account. */
    {
        pjsua_acc_config cfg;
        pjsua_acc_config_default(&cfg);
        cfg.register_on_acc_add = 1;
        cfg.ka_interval = 15;
        cfg.id = pj_str("sip:" SIP_USER "@" SIP_DOMAIN);
        cfg.reg_uri = pj_str("sip:" SIP_DOMAIN);
        cfg.cred_count = 1;
        cfg.cred_info[0].realm = pj_str(SIP_DOMAIN);
        cfg.cred_info[0].scheme = pj_str("digest");
        cfg.cred_info[0].username = pj_str(SIP_USER);
        cfg.cred_info[0].data_type = PJSIP_CRED_DATA_PLAIN_PASSWD;
        cfg.cred_info[0].data = pj_str(SIP_PASSWD);

        __android_log_print(ANDROID_LOG_VERBOSE, TAG, "pjsua_acc_adding");
        status = pjsua_acc_add(&cfg, PJ_TRUE, &acc_id);
        if (status != PJ_SUCCESS) error_exit("Error adding account", status);
        __android_log_print(ANDROID_LOG_VERBOSE, TAG, "pjsua_acc_added");

//        __android_log_print(ANDROID_LOG_VERBOSE, TAG, "sending registration manually");
//        status = pjsua_acc_set_registration(&acc_id, PJ_TRUE);
//        __android_log_print(ANDROID_LOG_VERBOSE, TAG, "sent registration manually");

    }

    return status;
}

JNIEXPORT jint Java_com_freetalk_pjinterface_PJ_registerPJClassJNI (JNIEnv* env, jobject obj)
{
    __android_log_print(ANDROID_LOG_VERBOSE, TAG, "registerPJClassJNI");
    jclass pjClass = (*env)->FindClass(env, "com/freetalk/pjinterface/PJ");
    g_obj = (*env)->NewGlobalRef(env, pjClass);
    return 1;
}

JNIEXPORT jint Java_com_freetalk_pjinterface_PJ_destroyEngineJNI(JNIEnv* env, jobject obj)
{
    __android_log_print(ANDROID_LOG_VERBOSE, TAG, "Destroy SIP Engine");
    int status = 0;
//    pj_pool_release(poolJNI);
//    poolJNI = NULL;
    status = pjsua_destroy();
    return status;
}

/* Callback called by the library upon receiving incoming call */
void on_incoming_call(pjsua_acc_id acc_id, pjsua_call_id call_id,
                             pjsip_rx_data *rdata)
{
    __android_log_print(ANDROID_LOG_VERBOSE, TAG, "on_incoming_call");
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
    __android_log_print(ANDROID_LOG_VERBOSE, TAG, "on_call_state");
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
    __android_log_print(ANDROID_LOG_VERBOSE, TAG, "pjsua_call_id");
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
    __android_log_print(ANDROID_LOG_VERBOSE, TAG, "error_exit");
    pjsua_perror(TAG, title, status);
    pjsua_destroy();
    exit(1);
}

#undef __PJAPI__




