#include <fightable/state.h>
#include <fightable/mp_server.h>
#include <fightable/storage.h>
#include <net_drivers/udp.h>

#ifndef _DISABLE_MP_SERVER_
#include <fightable/http/http_server.h>
#include <nbnet.h>
#endif

#ifdef TARGET_ANDROID
#include <android_native_app_glue.h>
#endif

extern void _fMainLog(const char* msg);

#define COMMAND_GET_UID         0x01
#define COMMAND_GET_USERS       0x02
#define COMMAND_ACKNOWLEDGE     '$'

#ifndef _DISABLE_MP_SERVER_
struct fmp_metadata_req *_fMpMetadataReqCreate(void) {
    return (struct fmp_metadata_req *)NBN_Allocator(sizeof(struct fmp_metadata_req));
}
void _fMpMetadataReqDestroy(struct fmp_metadata_req *obj) {
    if (obj) NBN_Deallocator(obj);
}
int _fMpMetadataReqSerialize(struct fmp_metadata_req *obj, NBN_Stream *stream) {
    if (!obj || !stream) return 0;

    NBN_SerializeUInt(stream, obj->http_port, 1024, 8000);
    NBN_SerializeUInt(stream, obj->max_players, 1, MP_MAX_CLIENTS);

    return 0;
}
#endif

unsigned char _fMpServerOpen() {
#ifndef _DISABLE_MP_SERVER_
    __state.mp_server_ready = 0;

    if (__state.mp_server_handles) {
        NBN_Deallocator(__state.mp_server_handles);
    }
    __state.mp_server_handle_amount = MP_MAX_CLIENTS;
    __state.mp_server_handles = (NBN_ConnectionHandle *)NBN_Allocator(sizeof(NBN_ConnectionHandle) * __state.mp_server_handle_amount);
    memset(__state.mp_server_handles, 0, sizeof(NBN_ConnectionHandle) * __state.mp_server_handle_amount);

    NBN_UDP_Register();

    unsigned char opened = 0;
    for (int i = 0; i < 16; i++) {
        __state.mp_server_port = GetRandomValue(1024, 8000);
        int status = NBN_GameServer_StartEx("fightable-0", __state.mp_server_port);

        if (status < 0) {
            TraceLog(LOG_ERROR, "Cannot start server on 0.0.0.0:%d (attempt %d) -> status=%d", (int)__state.mp_server_port, i, status);
        } else {
            TraceLog(LOG_INFO, "UDP server created successfully on 0.0.0.0:%d", __state.mp_server_port);
            opened = 1;
            break;
        }
    }

    if (!opened) {
        TraceLog(LOG_ERROR, "Critical failure on server creation");
        NBN_Deallocator(__state.mp_server_handles);
        __state.mp_server_handles = NULL;
        __state.mp_server_handle_amount = 0;
        return 0;
    }

    TraceLog(LOG_INFO, "Registering UDP messages");

    NBN_GameServer_RegisterMessage(MP_METADATA_REQ_ID,
        (NBN_MessageBuilder)_fMpMetadataReqCreate,
        (NBN_MessageDestructor)_fMpMetadataReqDestroy,
        (NBN_MessageSerializer)_fMpMetadataReqSerialize
    );

    TraceLog(LOG_INFO, "Creating HTTP server for resource downloading");
    __state.mp_server_http_port = __state.mp_server_port + 1;
    __state.webserver = _fHttpServerCreate(__state.mp_server_http_port, _fMainLog);
#ifdef TARGET_ANDROID
    _fHttpSetAllowedResourceDir(__state.webserver, __state.system->activity->internalDataPath);
#else
    _fHttpSetAllowedResourceDir(__state.webserver, _fStorageGetWritable());
#endif

    __state.mp_server_ready = 1;
    return 1;
#endif

    return 0;
}
