
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#ifndef _DISABLE_MP_SERVER_

#include <fightable/player_connection.h>
#include <fightable/state.h>

RSB_ARRAY_IMPL_GEN(struct fplayer_connection, _PlayerCon);

void _fMpInitPlayerList() {
    _fPlayerConnectionListRecreate(&__state.mp_connected_players);
}

struct fplayer_connection _fPlayerConnectionCreate() {
    struct fplayer_connection p = {};
    p.player_id = rand();
    return p;
}

void _fPlayerConnectionListCleanup(rsb_array__PlayerCon *list) {
    if (!list) return;

    for (unsigned int i = 0; i < list->len; i++) {
        struct fplayer_connection *con = list->objects + i;
        if (con->username) free(con->username);
    }

    RSBDestroy_PlayerCon(list);
}

void _fPlayerConnectionListRecreate(rsb_array__PlayerCon **list) {
    if (!list) return;

    _fPlayerConnectionListCleanup(*list);
    *list = RSBCreateArray_PlayerCon();
}

#endif
