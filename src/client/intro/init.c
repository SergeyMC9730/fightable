
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "fightable/level.h"
#include "fightable/sound_library.h"
#include "fightable/storage.h"
#include <fightable/intro.h>
#include <fightable/state.h>

#define SONGS_AVAILABLE 3

void _fIntroInit2(void *u) {
    TraceLog(LOG_INFO, "Doing second fade effect");
}

void _fIntroInit() {
    static const int song_ids[] = {
        MUS_ENDLESS_DREAM, MUS_ELECTRIC_DRUG, MUS_CELESTIAL_FANTASIA
    };
    __state.song_id = song_ids[GetRandomValue(0, (sizeof(song_ids) / sizeof(int)) - 1)];
    // __state.song_id = song_ids[2]; // TEMP

    const char *file_to_load = "EndlessDream.SymMOD";

    switch (__state.song_id) {
        case MUS_ELECTRIC_DRUG: {
            file_to_load = "electric_drug.mod";
            break;
        }
        case MUS_CELESTIAL_FANTASIA: {
            file_to_load = "celestial_fantasia.s3m";
            char *p = _fStorageFind("cf_level.bin");
            __state.current_level = _fLevelLoadFromFile(p);
            MemFree(p);
            if (__state.current_level) {
                UnloadTexture(__state.current_level->background_tile);
                Image ibt = GenImageColor(__state.tilemap->tile_size.x, __state.tilemap->tile_size.y, SKYBLUE);
                __state.current_level->background_tile = LoadTextureFromImage(ibt);
                UnloadImage(ibt);
            }
            __state.sound_engine.do_not_shake = 1;
            __state.display_test_midground = 1;
            break;
        }
    };

    char *p = _fStorageFind(file_to_load);
    _fAudioPlayModule(&__state.sound_engine, p);
    MemFree(p);


    if (__state.sound_engine.current_module) {
        _fAudioLoopCurrent(&__state.sound_engine);
    } else {
        __state.song_id = -1;
    }

    p = _fStorageFind("raylib_16x16.png");
    __state.raylib_logo = LoadTexture(p);
    MemFree(p);

    _fIntroGfxInit();
}
