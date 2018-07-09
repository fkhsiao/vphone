#   define PJ_IS_LITTLE_ENDIAN          1
#   define PJ_IS_BIG_ENDIAN             0

#include <pjsua.h>
#include <android/log.h>

#ifdef __PJAPI__
    pj_pool_t *poolJNI = NULL;

    JavaVM *g_vm;
    jobject g_obj;
    jmethodID g_midOnIncomingCall;
    jmethodID g_midOptionsCallback;
    int maxCalls;
    int maxRegs;
    int isPoint2Point;
    char hexMac[13];
    pj_bool_t has_etag;
    char instanceIdMac[50]; // example <urn:uuid:00000000-0000-1000-8000-AABBCCDDEEFF>
    unsigned acctFlags[PJSUA_MAX_ACC];
    int supportFallback;
#endif

#ifndef TAG
#define TAG "VP_SipServiceJNI"
#endif


void on_incoming_call(pjsua_acc_id acc_id, pjsua_call_id call_id, pjsip_rx_data *rdata);
void on_call_state(pjsua_call_id call_id, pjsip_event *e);
void on_call_media_state(pjsua_call_id call_id);
void error_exit(const char *title, pj_status_t status);



