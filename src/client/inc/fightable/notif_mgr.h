
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <fightable/notif_object.h>
#include <rsb/rsb_array_gen.h>

struct fnotif_mgr_entry {
    struct fnotif_object *popup;
    double time;
    unsigned char closing;
    int offset;
};

RSB_ARRAY_DEF_GEN(struct fnotif_mgr_entry, _fnotif_mgr_entry);

#ifdef __cplusplus
extern "C" {
#endif

void _fNotifMgrInit();
void _fNotifMgrSend(const char *message);
void _fNotifMgrUpdate();

#ifdef __cplusplus
}
#endif
