
//          Sergei Baigerov 2024 - 2026.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#ifndef _DISABLE_MP_SERVER_

#include <fightable/state.h>
#include <fightable/mp_client.h>
#include <fightable/storage.h>
#include <nbnet.h>
#include <net_drivers/udp.h>
#include <fightable/mp_server.h>
#include <fightable/notif_mgr.h>
#include <fightable/string.h>
#include <fightable/translation.h>

unsigned char _fMpClientConnect(const char* ip, unsigned short port) {
    TraceLog(LOG_INFO, "Connecting to %s:%d", ip, (int)port);

    if (!ip || port == 0) {
        _fNotifMgrSend(_fTranslationGetString("notification.mpc.assert"));

        if (__state.mp_client_notif_status) {
            __state.mp_client_notif_status->max_time = 1.f;
        }

        return 0;
    }

    ip = _fCopyString(ip);

    int res = NBN_GameClient_Start(MP_PROTOCOL, ip, port);
    if (res < 0) {
        _fNotifMgrSend(_fTranslationGetString("notification.mpc.basic_error"));

        if (__state.mp_client_notif_status) {
            __state.mp_client_notif_status->max_time = 1.f;
        }

        free(ip);

        return 0;
    } else {
        TraceLog(LOG_INFO, "Created connection to %s:%d", ip, port);
        _fMpRegisterMessages(1);
    }

    __state.mp_client_port = port;
    __state.mp_client_ip = ip;
    __state.mp_client_should_tick = 1;
    _fMpInitPlayerList();

    return 1;
}

#endif
