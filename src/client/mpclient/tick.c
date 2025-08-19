
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "fightable/mp_server_defs.h"
#include "fightable/text_input.h"
#include <fightable/mp_client.h>
#include <fightable/mp_server.h>
#include <fightable/notif_mgr.h>
#include <fightable/state.h>
#include <nbnet.h>
#include <fraylib.h>

void _fMpClientConnected() {
    _fNotifMgrSendWithTime("<cgreen,white>Connection\n<cgreen,white>established", 1.f);

    if (__state.mp_client_notif_status) {
        __state.mp_client_notif_status->popup->complete_progress = 0.25;
    }

    _fMpClientSendPacket(
        MP_CS_METADATA_ACQUIRE_ID,
        _fMpPacketCreateMetadataAcquire()
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

            _fNotifMgrSendWithTime("<cgreen,white>Acquired server\n<cgreen,white>metadata", 1.f);

            if (__state.mp_client_notif_status) {
                __state.mp_client_notif_status->popup->complete_progress = 0.5;
            }

            TraceLog(LOG_INFO, "HTTP port: %d; Max players: %d; Players connected: %d", data->http_port, data->max_players, data->players_connected);
            break;
        }
        default: {
            TraceLog(LOG_WARNING, "Unknown packet id %d", msg_info.type);
            break;
        }
    }
}

void _fMpClientTick() {
    int ev;

    while ((ev = NBN_GameClient_Poll()) != NBN_NO_EVENT) {
        if (ev < 0) {
            _fMpClientDisconnected();
            return;
        }

        switch (ev) {
            case NBN_CONNECTED: {
                _fMpClientConnected();
                break;
            }
            case NBN_DISCONNECTED: {
                _fMpClientDisconnected();
                break;
            }
            case NBN_MESSAGE_RECEIVED: {
                _fMpClientMsg();
                break;
            }
        }
    }
}
