
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#ifndef _DISABLE_MP_SERVER_

#include <fightable/mp_server_defs.h>
#include <nbnet.h>

#ifdef __cplusplus
extern "C" {
#endif

MP_CREATE_PACKET_CLASS_DEF(fmp_metadata_req, MetadataReq)
MP_CREATE_PACKET_CLASS_DEF(fmp_metadata_acquire, MetadataAcquire)

NBN_ConnectionHandle *_fMpServerFindHandle(NBN_ConnectionHandle ref);
unsigned char _fMpServerOpen();
void _fMpServerTick();

#ifdef __cplusplus
}
#endif

#endif
