
//          Sergei Baigerov 2024 - 2026.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "fightable/mp_shared.h"
#include "fightable/renderer.h"
#include "fightable/storage.h"
#ifndef _DISABLE_MP_SERVER_

#include "fightable/mp_packets.h"
#include "fightable/player_connection.h"
#include "fightable/string.h"
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
#include <fightable/level.h>
#include <fightable/translation.h>

struct fmp_client_download_ctx {
    char **assets;
    unsigned int assets_len;
};

void _fMpClientOnDownloadLevel2(void *_ctx) {
    TraceLog(LOG_INFO, "WORKS!!");

    struct fmp_client_download_ctx *ctx = (struct fmp_client_download_ctx *)_ctx;

    char *filename = ctx->assets[0];
    const char *base_folder = _fMpClientGetNewDirectory();
    const char *storage = _fStorageGetWritable();

    char *buffer = (char *)MemAlloc(1024);
    snprintf(buffer, 1024, "%s/%s/%s", storage, base_folder, filename);

    __state.mp_client_level = _fLevelLoadFromFile(buffer);

    free(ctx->assets);
    free(ctx);
}
void _fMpClientOnDownloadLevel(void *ctx) {
    _fScheduleOverlayFunc((renderer_event_t){_fMpClientOnDownloadLevel2, ctx});
}

void _fMpClientConnected() {
    _fNotifMgrSendWithTime(_fTranslationGetString("notification.mpc.connected"), 1.f);

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

    _fNotifMgrSend(_fTranslationGetString("notification.mpc.disconnected"));

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

            __state.mp_client_http_port = data->http_port;
            __state.mp_client_user_id = data->user_id;

            if (__state.mp_client_notif_status) {
                __state.mp_client_notif_status->popup->complete_progress = 1;
            }

            TraceLog(LOG_INFO, "HTTP port: %d; Max players: %d; Players connected: %d; User ID: %d", data->http_port, data->max_players, data->players_connected, data->user_id);

            _fMpCreateLobbyBackground();
            _fMpClientSwitchToLobby();

            break;
        }
        case MP_SC_OPENED_LEVEL: {
            struct fmp_opened_level *data = (struct fmp_opened_level *)msg_info.data;

            TraceLog(LOG_INFO, "Server wants client to download level %s", data->level_path);

            if (__state.mp_client_level) {
                unsigned char src = __state.mp_client_level->level_source;
                _fLevelDestroy(__state.mp_client_level, 1, (src != LEVEL_SOURCE_EDITOR), (src != LEVEL_SOURCE_EDITOR));
                __state.mp_client_level = NULL;
            }

            struct fmp_client_download_ctx *ctx = (struct fmp_client_download_ctx *)MemAlloc(sizeof(struct fmp_client_download_ctx));
            ctx->assets_len = 1;
            ctx->assets = (char **)MemAlloc(sizeof(char*) * ctx->assets_len);
            ctx->assets[0] = _fCopyString(data->level_path);

            __state.mp_client_http_port = data->http_port;

            _fMpClientDownloadAssets((const char **)ctx->assets, ctx->assets_len, _fMpClientOnDownloadLevel, ctx);

            break;
        }
        case MP_SC_PLAYER_LIST: {
            struct fmp_player_list *data = (struct fmp_player_list *)msg_info.data;

            __state.mp_client_user_id = data->receiver_user_id;
            _fMpInitPlayerList();

            for (unsigned int i = 0; i < sizeof(data->entries) / sizeof(*data->entries); i++) {
                struct fmp_player_list_e *entry = data->entries + i;

                if (!entry->is_connected) continue;

                struct fplayer_connection connection = {};
                connection.player_id = entry->user_id;
                connection.is_owner = entry->is_owner;
                connection.is_local = connection.player_id == data->receiver_user_id;
                connection.username = _fCopyString(entry->username);

                RSBAddElement_PlayerCon(__state.mp_connected_players, connection);
            }

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
