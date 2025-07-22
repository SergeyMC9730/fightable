
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <fightable/mp_server_defs.h>
#include <nbnet.h>

#ifdef __cplusplus
extern "C" {
#endif

struct fmp_metadata_req *_fMpMetadataReqCreate(void);
void _fMpMetadataReqDestroy(struct fmp_metadata_req *obj);
int _fMpMetadataReqSerialize(struct fmp_metadata_req *obj, NBN_Stream *stream);

NBN_ConnectionHandle *_fMpServerFindHandle(NBN_ConnectionHandle ref);

unsigned char _fMpServerOpen();

void _fMpServerTick();

#ifdef __cplusplus
}
#endif
