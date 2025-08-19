
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#ifndef _DISABLE_MP_SERVER_

#include <fightable/mp_shared.h>
#include <fightable/state.h>
#include <fightable/mp_client.h>
#include <fightable/mp_server.h>

void _fMpTick() {
    if (__state.mp_server_should_tick) {
        _fMpServerTick();
    }
    if (__state.mp_client_should_tick) {
        _fMpClientTick();
    }
}

#endif
