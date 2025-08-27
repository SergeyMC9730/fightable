
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#ifndef _DISABLE_MP_SERVER_

#include <fightable/state.h>
#include <fightable/mp_server.h>
#include <fightable/storage.h>
#include <net_drivers/udp.h>

void _fMpServerConnectHandle(NBN_ConnectionHandle h, char *username) {
    struct fplayer_connection connection = _fPlayerConnectionCreate();
    connection.srv_handler = h;
    connection.username = username;

    RSBAddElement_PlayerCon(__state.mp_connected_players, connection);

    _fMpServerSendPlayerList();

    if (__state.current_level) {
        struct fmp_opened_level *p = _fMpPacketCreateOpenedLevel();
        strcpy(p->level_path, "remote_level.bin");

        p->http_port = __state.mp_server_http_port;

        NBN_GameServer_SendReliableMessageTo(h, MP_SC_OPENED_LEVEL, p);
    }
}

#endif
