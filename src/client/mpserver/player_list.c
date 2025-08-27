
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#ifndef _DISABLE_MP_SERVER_

#include <fightable/state.h>
#include <fightable/mp_server.h>
#include <fightable/storage.h>
#include <net_drivers/udp.h>

void _fMpServerSendPlayerList() {
    struct fmp_player_list *packet_template = _fMpPacketCreatePlayerList();

    for (unsigned int i = 0; i < __state.mp_connected_players->len && i < MP_MAX_CLIENTS; i++) {
        struct fplayer_connection *con = __state.mp_connected_players->objects + i;
        struct fmp_player_list_e *entry = packet_template->entries + i;

        if (con->username == NULL) continue;

        entry->is_connected = 1;
        entry->is_owner = con->is_owner;
        entry->user_id = con->player_id;
        strncpy(entry->username, con->username, 8);
        entry->username[sizeof(entry->username) - 1] = 0;
    }

    for (unsigned int i = 0; i < __state.mp_connected_players->len; i++) {
        struct fplayer_connection *con = __state.mp_connected_players->objects + i;
        if (con->srv_handler == 0) continue;

        struct fmp_player_list *packet = _fMpPacketCreatePlayerList();
        memcpy(packet, packet_template, sizeof(*packet));

        packet->receiver_user_id = con->player_id;

        NBN_GameServer_SendReliableMessageTo(con->srv_handler, MP_SC_PLAYER_LIST, packet);
    }

    _fMpPacketDestroyPlayerList(packet_template);
}

#endif
