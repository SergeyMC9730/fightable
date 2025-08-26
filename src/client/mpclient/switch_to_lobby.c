
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#ifndef _DISABLE_MP_SERVER_

#include <fightable/gfx.h>
#include <fightable/state.h>
#include <fightable/storage.h>
#include <fightable/sound_library.h>

#include <stdio.h>

void _fMpClientSwitchToLobby2(void *ctx) {
    _fAudioStop(&__state.sound_engine);

    char* buffer = (char*)MemAlloc(256);
    const char* readable = _fStorageGetWritable();

    _fAudioStop(&__state.sound_engine);

    snprintf(buffer, 256, "%s/3g_crim.xm", readable);

    _fAudioPlayModule(&__state.sound_engine, buffer);
    _fAudioLoopCurrent(&__state.sound_engine);

    MemFree(buffer);

    __state.song_id = MUS_3G_ANTS;
    __state.current_ui_menu = UI_MENU_MPJOIN;

    _fAudioSetVolume(&__state.sound_engine, 0.f);
    _fAudioFxSlideVolume(&__state.sound_engine, __state.config.volume_slider.progress, 0.5f);
}

void _fMpClientSwitchToLobby() {
    _fGfxFadeOut(&__state.gfx, BLACK, 0.5);
    _fGfxSetFadeOutFunc(&__state.gfx, _fMpClientSwitchToLobby2, NULL);
}

#endif
