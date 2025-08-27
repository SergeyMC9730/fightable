
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#ifndef _DISABLE_MP_SERVER_

#include <fightable/mp_shared.h>
#include <stdbool.h>

typedef char fmp_username_field[0x20 + 1];

struct fmp_metadata_req {
    bool connection_rejected;

    unsigned int http_port;
    unsigned int max_players;
    unsigned int players_connected;

    int user_id;
};
struct fmp_metadata_acquire {
    fmp_username_field username;
};
struct fmp_heartbeat {
    char unused[1];
};
struct fmp_opened_level {
    char level_path[0xFF + 1];
    unsigned int http_port;
};
struct fmp_player_list_e {
    fmp_username_field username;
    unsigned char is_owner;
    unsigned char is_connected;
    int user_id;
};
struct fmp_player_list {
    struct fmp_player_list_e entries[MP_MAX_CLIENTS];
    int receiver_user_id;
};

#define MP_SC_METADATA_REQ_ID                   0x00
#define MP_CS_METADATA_ACQUIRE_ID               0x01
#define MP_SC_HEARTBEAT                         0x02
#define MP_SC_OPENED_LEVEL                      0x03
#define MP_SC_PLAYER_LIST                       0x04

#endif
