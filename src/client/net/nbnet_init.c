
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/mp_shared.h>
#include <nbnet.h>
#include <net_drivers/udp.h>
#include <fraylib.h>

void _fMpInit() {
    TraceLog(LOG_INFO, "Registering UDP driver from nbnet");
    NBN_UDP_Register();
}
