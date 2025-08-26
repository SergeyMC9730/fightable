
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#ifndef _DISABLE_MP_SERVER_

#include "fightable/mp_shared.h"
#include "fightable/player_connection.h"
#include "fightable/string.h"

#include <fightable/mp_server_defs.h>
#include <fightable/mp_server.h>
#include <fightable/state.h>

void _fMpServerTickAcceptConnection() {
    unsigned int slots_available = __state.mp_server_max_connections - __state.mp_connected_players->len;

    if (slots_available == 0) {
        TraceLog(LOG_ERROR, "Cannot accept UDP connection: too much clients are connected");
        NBN_GameServer_RejectIncomingConnectionWithCode(MP_CON_REJECT_BUSY);
        return;
    }

    NBN_GameServer_AcceptIncomingConnection();
    NBN_ConnectionHandle handle = NBN_GameServer_GetIncomingConnection();

    struct fplayer_connection connection = {};
    connection.username = NULL;
    connection.linked_entity = NULL;
    connection.srv_handler = handle;

    RSBAddElement_PlayerCon(__state.mp_connected_players, connection);

    TraceLog(LOG_INFO, "Accepted client %d", handle);
}
void _fMpServerTickAcceptDisconnect() {
    struct fmp_find_handle_result result = _fMpServerFindHandle(NBN_GameServer_GetDisconnectedClient());;

    if (!result.success) {
        TraceLog(LOG_WARNING, "Disconnected user cannot be found on the server");
        return;
    }
}

void _fMpServerTickOnMessage() {
    NBN_MessageInfo info = NBN_GameServer_GetMessageInfo();

    struct fmp_find_handle_result result = _fMpServerFindHandle(info.sender);

    if (!result.success) {
        TraceLog(LOG_ERROR, "MP server received message from unknown user");
        return;
    }

    switch (info.type) {
        case MP_CS_METADATA_ACQUIRE_ID: {
            struct fmp_metadata_acquire *a = (struct fmp_metadata_acquire *)info.data;

            TraceLog(LOG_INFO, "Setting %d's username to: %s", info.sender, a->username);
            result.ref->username = _fCopyString(a->username);

            struct fmp_metadata_req *r = _fMpPacketCreateMetadataReq();
            r->http_port = __state.mp_server_http_port;
            r->max_players = MP_MAX_CLIENTS;
            r->players_connected = __state.mp_connected_players->len;
            r->connection_rejected = r->players_connected == (r->max_players + 1);

            NBN_GameServer_SendReliableMessageTo(info.sender, MP_SC_METADATA_REQ_ID, r);
            // _fMpPacketDestroyMetadataReq(r);

            break;
        }
        default: {
            TraceLog(LOG_WARNING, "Unknown packet %d", (int)info.type);
            break;
        }
    }
}

void _fMpServerSendHeartbeat() {
    for (unsigned int i = 0; i < __state.mp_connected_players->len; i++) {
        struct fplayer_connection *con = __state.mp_connected_players->objects + i;

        if (con->srv_handler == 0) continue;

        struct fmp_heartbeat *packet = _fMpPacketCreateHeartbeat();
        NBN_GameServer_SendUnreliableMessageTo(con->srv_handler, MP_SC_HEARTBEAT, packet);
        // NBN_Deallocator(packet);
    }
}

void _fMpServerTick() {
    // if (!__state.mp_server_ready) return;

    int ev = NBN_GameServer_Poll();

    while (ev != NBN_NO_EVENT) {
        TraceLog(LOG_INFO, "EV2: %d", ev);

        if (ev < 0) {
            TraceLog(LOG_ERROR, "_fMpServerTick: cannot get event");
        }

        switch (ev) {
            case NBN_NEW_CONNECTION: {
                _fMpServerTickAcceptConnection();
                break;
            }
            case NBN_DISCONNECTED: {
                _fMpServerTickAcceptDisconnect();
                break;
            }
            case NBN_MESSAGE_RECEIVED: {
                _fMpServerTickOnMessage();
                break;
            }
        }

        ev = NBN_GameServer_Poll();
    }

    NBN_GameServer_SendPackets();
}

#endif
