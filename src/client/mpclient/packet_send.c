
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/mp_client.h>
#include <nbnet.h>
#include <fraylib.h>

void _fMpClientSendPacket(unsigned char type, void *data) {
    TraceLog(LOG_INFO, "Sending packet %d", (int)type);
    NBN_GameClient_SendReliableMessage(type, data);
}
