
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#ifndef _DISABLE_MP_SERVER_

#include <fightable/state.h>
#include <fightable/mp_client.h>
#include <fightable/storage.h>
#include <net_drivers/udp.h>
#include <fightable/notif_mgr.h>

unsigned char _fMpClientConnect(const char* ip, unsigned short port) {
    if (!ip || port == 0 || true) {
        _fNotifMgrSend("<cred,orange>ERROR:\n<cyellow>Pre-connection assertion failed!");

        if (__state.mp_client_notif_status) {
            __state.mp_client_notif_status->max_time = 1.f;
        }

        return 0;
    }

    return 1;
}

#endif
