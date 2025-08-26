
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#ifndef _DISABLE_MP_SERVER_

#include "fightable/intro.h"
#include "fightable/mp_server_defs.h"
#include "fightable/text_input.h"
#include <fightable/mp_client.h>
#include <fightable/mp_server.h>
#include <fightable/notif_mgr.h>
#include <fightable/state.h>
#include <nbnet.h>
#include <fraylib.h>
#include <fightable/curl_frontend.h>

void _fMpClientTEST(struct fcurl_con_settings *s) {
    TraceLog(LOG_INFO, "WORKS!!");
    fclose(s->output_file);
}

void _fMpClientConnected() {
    _fNotifMgrSendWithTime("<cgreen,white>Connection\n<cgreen,white>established", 1.f);

    if (__state.mp_client_notif_status) {
        __state.mp_client_notif_status->popup->complete_progress = 0.25;
    }

    struct fmp_metadata_acquire *packet = _fMpPacketCreateMetadataAcquire();
    memcpy(packet->username, __state.name_input->buffer, 32);
    packet->username[32] = 0;

    _fMpClientSendPacket(
        MP_CS_METADATA_ACQUIRE_ID,
        packet
    );
}
void _fMpClientDisconnected() {
    __state.mp_client_connecting = 0;
    __state.mp_client_should_tick = 0;

    _fNotifMgrSend("Disconnected\nfrom server");

    if (__state.mp_client_notif_status) {
        __state.mp_client_notif_status->time = __state.mp_client_notif_status->popup->complete_progress;
        __state.mp_client_notif_status->max_time = 1.f;
    }

    if (__state.ip_input) {
        _fTextInputUnlock(__state.ip_input);
    }
    if (__state.name_input) {
        _fTextInputUnlock(__state.name_input);
    }
}
void _fMpClientMsg() {
    NBN_MessageInfo msg_info = NBN_GameClient_GetMessageInfo();

    switch (msg_info.type) {
        case MP_SC_METADATA_REQ_ID: {
            struct fmp_metadata_req *data = (struct fmp_metadata_req *)msg_info.data;

            __state.mp_client_srvmeta = data;

            if (__state.mp_client_notif_status) {
                __state.mp_client_notif_status->popup->complete_progress = 0.5;
            }

            TraceLog(LOG_INFO, "HTTP port: %d; Max players: %d; Players connected: %d", data->http_port, data->max_players, data->players_connected);

            _fIntroMenuInitMpBackground();
            _fMpClientSwitchToLobby();

            break;
        }
        case MP_SC_OPENED_LEVEL: {
            struct fmp_opened_level *data = (struct fmp_opened_level *)msg_info.data;

            TraceLog(LOG_INFO, "Server wants client to download level %s", data->level_path);

            break;
        }
        default: {
            TraceLog(LOG_WARNING, "Unknown packet id %d", msg_info.type);
            break;
        }
    }
}

void _fMpClientTick() {
    int ev = NBN_GameClient_Poll();

    while (ev != NBN_NO_EVENT) {
        TraceLog(LOG_INFO, "EV2: %d", ev);

        if (ev < 0 || ev == NBN_DISCONNECTED) {
            _fMpClientDisconnected();
            return;
        }

        switch (ev) {
            case NBN_CONNECTED: {
                _fMpClientConnected();
                break;
            }
            case NBN_MESSAGE_RECEIVED: {
                _fMpClientMsg();
                break;
            }
        }

        ev = NBN_GameClient_Poll();
    }

    NBN_GameClient_SendPackets();
}

#endif
