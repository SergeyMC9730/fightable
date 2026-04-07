
//          Sergei Baigerov 2024 - 2026.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "fightable/mp_server.h"
#include "fightable/mp_shared.h"
#include <fraylib.h>
#include <fightable/intro.h>
#include <fightable/state.h>
#include <stdio.h>
#include <fightable/button.h>
#include <fightable/rect.h>
#include <fightable/renderer.h>
#include <fightable/level.h>
#include <fightable/editor.h>
#include <fightable/storage.h>
#include <fightable/sound_library.h>
#include <fightable/mp_create_menu.h>
#include <fightable/compile_config.h>
#include <fightable/singleplayer.h>

#ifndef _DISABLE_MP_SERVER_
void _fIntroMenuOnMpCreateCallback(void *ctx) {
    if (!_fMpServerOpen()) return;

    float* old_vol = (float*)ctx;
    if (!old_vol) return;

    char* buffer = (char*)MemAlloc(256);
    const char* readable = _fStorageGetWritable();

    _fAudioStop(&__state.sound_engine);

    _fMpCreateLobbyBackground();

    snprintf(buffer, 256, "%s/assets/music/3g_crim.xm", readable);

    _fAudioPlayModule(&__state.sound_engine, buffer);
    _fAudioLoopCurrent(&__state.sound_engine);
    _fAudioFxSlideVolume(&__state.sound_engine, *old_vol, 0.5f);

    MemFree(old_vol);
    MemFree(buffer);

    __state.song_id = MUS_3G_ANTS;
    __state.current_ui_menu = UI_MENU_MPCREATE;
    __state.mp_create_time = 0;
}
#endif

void _fIntroMenuOnMpCreate() {
#ifdef DEMO_MODE
    _fSingleplayerJoinLobby();
    return;
#endif
#ifndef _DISABLE_MP_SERVER_
    float* old_vol = (float *)MemAlloc(sizeof(float));
    *old_vol = _fAudioGetVolume(&__state.sound_engine);

    _fGfxFadeOut(&__state.gfx, BLACK, 0.5);
    _fGfxSetFadeOutFunc(&__state.gfx, _fIntroMenuOnMpCreateCallback, old_vol);

    _fAudioFxSlideVolume(&__state.sound_engine, 0.f, 0.4f);
#endif
}
