
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#ifndef _DISABLE_MP_SERVER_

#include "fightable/mp_shared.h"
#include <fightable/mp_server.h>
#include <stdio.h>
#include <unistd.h>
#include <fightable/level.h>
#include <fightable/state.h>
#include <fightable/renderer.h>
#include <fightable/storage.h>

void _fMpOnOpenLevel(struct nt_file_selector_menu *ctx, const char *path) {
    char *filename = "remote_level.bin";

    struct fmp_opened_level *p = _fMpPacketCreateOpenedLevel();
    snprintf(p->level_path, sizeof(p->level_path) - 1, "%s/%s", _fStorageGetWritable(), filename);

    remove(p->level_path);
    link(path, p->level_path);

    struct flevel *lvl = _fLevelLoadFromFileSelector(p->level_path);

    if (lvl != __state.current_level && __state.current_level) {
        unsigned char src = __state.current_level->level_source;
        _fLevelDestroy(__state.current_level, 1, (src != LEVEL_SOURCE_EDITOR), (src != LEVEL_SOURCE_EDITOR));
        __state.current_level = NULL;
    }

    __state.current_level = lvl;

    memset(p->level_path, 0, sizeof(p->level_path));
    strcpy(p->level_path, filename);

    for (unsigned int i = 0; i < __state.mp_connected_players->len; i++) {
        struct fplayer_connection *con = __state.mp_connected_players->objects + i;
        if (con->srv_handler == 0) continue;

        NBN_GameServer_SendReliableMessageTo(con->srv_handler, MP_SC_OPENED_LEVEL, p);
    }

    NBN_Deallocator(p);

    _fCloseFileSelector();
}

#endif
