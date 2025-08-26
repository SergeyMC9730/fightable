
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#ifndef _DISABLE_MP_SERVER_

#include <fightable/mp_server_defs.h>
#include <nt5emul/tui/file_selector.h>
#include <nbnet.h>

#ifdef __cplusplus
extern "C" {
#endif

MP_CREATE_PACKET_CLASS_DEF(fmp_metadata_req, MetadataReq)
MP_CREATE_PACKET_CLASS_DEF(fmp_metadata_acquire, MetadataAcquire)
MP_CREATE_PACKET_CLASS_DEF(fmp_heartbeat, Heartbeat)
MP_CREATE_PACKET_CLASS_DEF(fmp_opened_level, OpenedLevel)

struct fplayer_connection;

struct fmp_find_handle_result {
    unsigned char success;
    unsigned int index;
    struct fplayer_connection *ref;
};

struct fmp_find_handle_result _fMpServerFindHandle(NBN_ConnectionHandle ref);
unsigned char _fMpServerOpen();
void _fMpServerTick();

void _fMpOnOpenLevel(struct nt_file_selector_menu *ctx, const char *path);

#ifdef __cplusplus
}
#endif

#endif
