
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "fightable/mp_packets.h"
#ifndef _DISABLE_MP_SERVER_

#include "fightable/mp_server_defs.h"

#include <fightable/mp_shared.h>
#include <fightable/state.h>
#include <fightable/mp_client.h>
#include <fightable/mp_server.h>

MP_CREATE_PACKET_CLASS_IMPL(fmp_metadata_req, MetadataReq,
    NBN_SerializeBool(stream, obj->connection_rejected);
    NBN_SerializeUInt(stream, obj->http_port, 0x400, 0xFFFF);
    NBN_SerializeUInt(stream, obj->max_players, 1, MP_MAX_CLIENTS);
    NBN_SerializeUInt(stream, obj->players_connected, 0, MP_MAX_CLIENTS);
    NBN_SerializeInt(stream, obj->user_id, INT_MIN, INT_MAX);
)
MP_CREATE_PACKET_CLASS_IMPL(fmp_metadata_acquire, MetadataAcquire,
    MP_PACKET_SERIALIZE_ARRAY(obj->username);
)
MP_CREATE_PACKET_CLASS_IMPL(fmp_heartbeat, Heartbeat,
    MP_PACKET_SERIALIZE_ARRAY(obj->unused);
)
MP_CREATE_PACKET_CLASS_IMPL(fmp_opened_level, OpenedLevel,
    MP_PACKET_SERIALIZE_ARRAY(obj->level_path);
    NBN_SerializeUInt(stream, obj->http_port, 0x400, 0xFFFF);
)
MP_CREATE_PACKET_CLASS_IMPL(fmp_player_list, PlayerList,
    NBN_SerializeBytes(stream, obj->entries, sizeof(obj->entries));
    NBN_SerializeInt(stream, obj->receiver_user_id, INT_MIN, INT_MAX);
)

void _fMpRegisterMessages(unsigned char is_client) {
    TraceLog(LOG_INFO, "Registering UDP messages");

    MP_PACKET_CLASS_ATTACH(is_client, MP_SC_METADATA_REQ_ID, MetadataReq);
    MP_PACKET_CLASS_ATTACH(is_client, MP_CS_METADATA_ACQUIRE_ID, MetadataAcquire);
    MP_PACKET_CLASS_ATTACH(is_client, MP_SC_HEARTBEAT, Heartbeat);
    MP_PACKET_CLASS_ATTACH(is_client, MP_SC_OPENED_LEVEL, OpenedLevel);
    MP_PACKET_CLASS_ATTACH(is_client, MP_SC_PLAYER_LIST, PlayerList);
}

#endif
