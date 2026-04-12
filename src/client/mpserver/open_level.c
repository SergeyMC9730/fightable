
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#ifndef _DISABLE_MP_SERVER_

#include "fightable/mp_shared.h"
#include <fightable/mp_server.h>
#include <stdio.h>
#ifdef TARGET_UNIX
#include <unistd.h>
#else
#include <fightable/filesystem.h>
#endif
#include <fightable/level.h>
#include <fightable/state.h>
#include <fightable/renderer.h>
#include <fightable/storage.h>

void _fMpOnOpenLevel(struct nt_file_selector_menu *ctx, const char *path) {
    char *filename = "remote_level.bin";

    struct fmp_opened_level *packet_template = _fMpPacketCreateOpenedLevel();
    snprintf(packet_template->level_path, sizeof(packet_template->level_path), "%s/%s", _fStorageGetWritable(), filename);

    packet_template->http_port = __state.mp_server_http_port;

    remove(packet_template->level_path);
#ifdef TARGET_UNIX
    link(path, packet_template->level_path);
#else
    _fFsFileCopy(path, packet_template->level_path);
#endif

    struct flevel *lvl = _fLevelLoadFromFileSelector(packet_template->level_path);

    if (lvl != __state.current_level && __state.current_level) {
        unsigned char src = __state.current_level->level_source;
        _fLevelDestroy(__state.current_level, 1, (src != LEVEL_SOURCE_EDITOR), (src != LEVEL_SOURCE_EDITOR));
        __state.current_level = NULL;
    }

    __state.current_level = lvl;

    memset(packet_template->level_path, 0, sizeof(packet_template->level_path));
    strcpy(packet_template->level_path, filename);

    for (unsigned int i = 0; i < __state.mp_connected_players->len; i++) {
        struct fplayer_connection *con = __state.mp_connected_players->objects + i;
        if (con->srv_handler == 0) continue;

        struct fmp_opened_level *packet = _fMpPacketCreateOpenedLevel();
        memcpy(packet, packet_template, sizeof(*packet));

        NBN_GameServer_SendReliableMessageTo(con->srv_handler, MP_SC_OPENED_LEVEL, packet);
    }

    // NBN_Deallocator(p);

    _fCloseFileSelector();
}

#endif
