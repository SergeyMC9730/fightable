
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#ifndef _DISABLE_MP_SERVER_

#include <nbnet.h>

#define MP_PROTOCOL     "fightable-0"
#define MP_MAX_CLIENTS  8

struct ferplayer;
struct fmp_obj_player {
    NBN_ConnectionHandle handle;
    struct ferplayer *linked_entity;
    char *username;
};

#ifdef __cplusplus
extern "C" {
#endif

void _fMpTick();
void _fMpInit();
void _fMpRegisterMessages(unsigned char is_client);

void _fMpDrawLobby(float opacity, unsigned char interactable);
void _fMpDrawLobbyBackground(float opacity);

void _fMpInitPlayerList();

#ifdef __cplusplus
}
#endif

#endif
