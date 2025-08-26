
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#ifndef _DISABLE_MP_SERVER_

#include <nbnet.h>
#include <rsb/rsb_array_gen.h>

struct ferplayer;
struct fplayer_connection {
    NBN_ConnectionHandle srv_handler; // required on server-side only
    struct ferplayer *linked_entity; // optional; can be NULL

    char *username;
};

RSB_ARRAY_DEF_GEN(struct fplayer_connection, _PlayerCon);

#endif
