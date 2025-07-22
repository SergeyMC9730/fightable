
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/mp_server.h>
#include <fightable/state.h>

void _fMpServerTickAcceptConnection() {
    NBN_ConnectionHandle *available_handle = _fMpServerFindHandle(0);

    if (!available_handle) {
        TraceLog(LOG_ERROR, "Cannot accept UDP connection: too much clients are connected");
        NBN_GameServer_RejectIncomingConnectionWithCode(MP_CON_REJECT_BUSY);
        return;
    }

    NBN_GameServer_AcceptIncomingConnection();
    *available_handle = NBN_GameServer_GetIncomingConnection();

    TraceLog(LOG_INFO, "Accepted client %d", *available_handle);
}
void _fMpServerTickAcceptDisconnect() {
    NBN_ConnectionHandle *handle_ref = _fMpServerFindHandle(NBN_GameServer_GetDisconnectedClient());

    if (!handle_ref) {
        TraceLog(LOG_WARNING, "Disconnected user is not found on the server");
        return;
    }

    *handle_ref = 0;
}

void _fMpServerTick() {
#ifndef _DISABLE_MP_SERVER_
    if (!__state.mp_server_ready) return;

    int ev;

    while ((ev = NBN_GameServer_Poll()) != NBN_NO_EVENT) {
        if (ev < 0) {
            TraceLog(LOG_ERROR, "_fMpServerTick: cannot get event");
            return;
        }

        switch (ev) {
            case NBN_NEW_CONNECTION: {
                _fMpServerTickAcceptConnection();
                break;
            }
        }
    }
#endif
}
