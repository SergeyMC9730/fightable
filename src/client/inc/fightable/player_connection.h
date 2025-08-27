
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#ifndef _DISABLE_MP_SERVER_

#include <nbnet.h>
#include <rsb/rsb_array_gen.h>

struct ferplayer;
struct felplayer;

struct fplayer_connection {
    NBN_ConnectionHandle srv_handler; // required on server-side only
    struct ferplayer *linked_entity_a; // optional; can be NULL
    struct felplayer *linked_entity_b; // optional; can be NULL

    unsigned char is_local;
    unsigned char is_owner;

    int player_id;

    char *username;
};

#ifdef __cplusplus
extern "C" {
#endif

RSB_ARRAY_DEF_GEN(struct fplayer_connection, _PlayerCon);

struct fplayer_connection _fPlayerConnectionCreate();

void _fPlayerConnectionListCleanup(rsb_array__PlayerCon *list);
void _fPlayerConnectionListRecreate(rsb_array__PlayerCon **list);

#ifdef __cplusplus
}
#endif

#endif
