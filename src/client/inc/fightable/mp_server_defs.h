
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

struct fmp_metadata_req {
    unsigned int http_port;
    unsigned int max_players;
};

#define MP_METADATA_REQ_ID 0x00


#define MP_CON_REJECT_BUSY 0x00


#define MP_MAX_CLIENTS 8
