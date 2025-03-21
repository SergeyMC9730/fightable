
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fraylib.h>
#include <fightable/intro.h>
#include <fightable/state.h>
#include <fightable/debug.h>

#ifdef DEBUG
static unsigned char __dbg_continue = 0;
#endif

void _fIntroDraw() {
    // ClearBackground(BLACK);

    _fIntroProcessGfx();

    double t = _fAudioGetPlayTime(&__state.sound_engine);

#ifdef DEBUG
    char buffer[48] = {};
    snprintf(buffer, 48, "t: %f\n%d", (float)t, __state.title_song_stage);

    /**
     * Step at 1.080000
     * Step at 1.440000
     * Step at 1.790000
     * Step at 3.960000
    */

    _fTextDraw(&__state.text_manager, buffer, (IVector2){1, 1}, GREEN, 1);

    if (IsKeyPressed(KEY_ENTER)) {
        __dbg_continue = 1;
        __state.title_song_stage = 0;
        _fIntroInit();
    }

    if (!__dbg_continue) {
        _fTextDraw(&__state.text_manager, "enter to continue", (IVector2){1, 20}, YELLOW, 1);

        return;
    }
#endif

    if (IsKeyPressed(KEY_P)) {
        TraceLog(LOG_INFO, "Playtime: %f", (float)t);
    }

    switch (__state.song_id) {
        case 0: {
            _fIntroProcessEndlessDream();
            break;
        }
        case 1: {
            _fIntroProcessElectricDrug();
            break;
        }
    }

    return;
}
