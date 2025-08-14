
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#ifndef _DISABLE_MP_SERVER_

#include <fightable/mp_server.h>
#include <fightable/state.h>

NBN_ConnectionHandle *_fMpServerFindHandle(NBN_ConnectionHandle ref) {
    NBN_ConnectionHandle *handle_ref = NULL;

    for (unsigned int i = 0; i < __state.mp_server_handle_amount && __state.mp_server_handles != NULL; i++) {
        if (*(__state.mp_server_handles + i) == ref) {
            handle_ref = __state.mp_server_handles + i;
            break;
        }
    }

    return handle_ref;
}

#endif
