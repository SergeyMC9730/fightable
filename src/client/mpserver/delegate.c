
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/state.h>
#include <fightable/mp_server.h>
#include <fightable/storage.h>
#include <net_drivers/udp.h>

#ifndef _DISABLE_MP_SERVER_
#include <fightable/http/http_server.h>
#include <nbnet.h>
#endif

#ifndef _DISABLE_MP_SERVER_
MP_CREATE_PACKET_CLASS_IMPL(fmp_metadata_req, MetadataReq,
    NBN_SerializeUInt(stream, obj->http_port, 1024, 8000);
    NBN_SerializeUInt(stream, obj->max_players, 1, MP_MAX_CLIENTS);
    NBN_SerializeUInt(stream, obj->players_connected, 0, MP_MAX_CLIENTS);
)
MP_CREATE_PACKET_CLASS_IMPL(fmp_metadata_acquire, MetadataAcquire,
    NBN_SerializeBytes(stream, &obj->pad, 1);
)
#endif

extern void _fMainLog(const char *msg);

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
        __state.mp_server_port = 3000; // TEMP
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

    MP_PACKET_CLASS_ATTACH(MP_METADATA_REQ_ID, MetadataReq);
    MP_PACKET_CLASS_ATTACH(MP_METADATA_ACQUIRE_ID, MetadataAcquire);

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
