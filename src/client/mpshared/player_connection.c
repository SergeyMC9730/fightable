
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#ifndef _DISABLE_MP_SERVER_

#include <fightable/player_connection.h>
#include <fightable/state.h>

RSB_ARRAY_IMPL_GEN(struct fplayer_connection, _PlayerCon);

void _fMpInitPlayerList() {
    if (__state.mp_connected_players) {
        RSBDestroy_PlayerCon(__state.mp_connected_players);
    }
    __state.mp_connected_players = RSBCreateArray_PlayerCon();
}

#endif
