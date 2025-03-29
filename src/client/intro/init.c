
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/intro.h>
#include <fightable/state.h>

#define SONGS_AVAILABLE 3

void _fIntroInit() {
    static int song_ids[] = {
        0, 1, 3
    };
    __state.song_id = song_ids[GetRandomValue(0, (sizeof(song_ids) / sizeof(int)) - 1)];
    __state.song_id = song_ids[2]; // TEMP

    const char *file_to_load = "EndlessDream.SymMOD";

    switch (__state.song_id) {
        case 1: {
            file_to_load = "electric_drug.mod";
            break;
        }
        case 3: {
            file_to_load = "celestial_fantasia.s3m";
            break;
        }
    };

    _fAudioPlayModule(&__state.sound_engine, file_to_load);
    if (!__state.sound_engine.current_module) {
        __state.song_id = -1;
    } else {
        _fAudioLoopCurrent(&__state.sound_engine);
    }

    __state.raylib_logo = LoadTexture("raylib_16x16.png");

    _fIntroGfxInit();
}
