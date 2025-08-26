
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#ifndef _DISABLE_MP_SERVER_

#include <fightable/mp_server.h>
#include <fightable/state.h>

struct fmp_find_handle_result _fMpServerFindHandle(NBN_ConnectionHandle ref) {
    struct fmp_find_handle_result res = {};

    if (__state.mp_connected_players == NULL || ref == 0) return res;

    for (unsigned int i = 0; i < __state.mp_connected_players->len; i++) {
        struct fplayer_connection *con = __state.mp_connected_players->objects + i;
        if (con->srv_handler == ref) {
            res.index = i;
            res.ref = con;
            res.success = true && res.ref;

            return res;
        }
    }

    return res;
}

#endif
