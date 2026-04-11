
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#ifndef _DISABLE_MP_SERVER_

#include "fightable/string.h"
#include "fightable/mp_server_defs.h"
#include "fightable/mp_shared.h"
#include "fightable/player_connection.h"

#include <fightable/state.h>
#include <fightable/mp_server.h>
#include <fightable/storage.h>
#include <fightable/level.h>
#include <net_drivers/udp.h>

#include <fightable/http/http_server.h>
#include <nbnet.h>

#include <stdio.h>

#ifdef TARGET_ANDROID
#include <android_native_app_glue.h>
#endif

extern void _fMainLog(const char *msg);

unsigned char _fMpServerOpen() {
    __state.mp_server_ready = 0;

    _fMpInitPlayerList();
    __state.mp_server_max_connections = MP_MAX_CLIENTS;

    unsigned char opened = 0;
    for (int i = 0; i < 16; i++) {
        __state.mp_server_port = GetRandomValue(0x0400, 0x3FFF);
        int status = NBN_GameServer_Start(MP_PROTOCOL, __state.mp_server_port);

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
        RSBDestroy_PlayerCon(__state.mp_connected_players);
        return 0;
    }

    _fMpRegisterMessages(0);

    TraceLog(LOG_INFO, "Creating HTTP server for resource downloading");
    __state.mp_server_http_port = __state.mp_server_port + 1;
    __state.webserver = _fHttpServerCreate(__state.mp_server_http_port, _fMainLog);
#ifdef TARGET_ANDROID
    _fHttpSetAllowedResourceDir(__state.webserver, __state.system->activity->internalDataPath);
#else
    _fHttpSetAllowedResourceDir(__state.webserver, _fStorageGetWritable());
#endif

    struct fplayer_connection connection = _fPlayerConnectionCreate();
    connection.username = _fCopyString("Owner");
    connection.is_local = 1;
    connection.is_owner = 1;

    RSBAddElement_PlayerCon(__state.mp_connected_players, connection);

    if (__state.current_level) {
        char *buffer = (char *)MemAlloc(512);
        char *filename = "remote_level.bin";

        snprintf(buffer, 512, "%s/%s", _fStorageGetWritable(), filename);

        _fLevelSave(__state.current_level, buffer);
    }

    __state.mp_server_ready = 1;
    __state.mp_server_should_tick = 1;

    return 1;
}

#endif
