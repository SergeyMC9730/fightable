#include <fightable/intro.h>
#include <fightable/state.h>
#include <stdio.h>

#define SONGS_AVAILABLE 2

void _fIntroInit() {
    __state.song_id = GetRandomValue(0, SONGS_AVAILABLE - 1);

    const char *file_to_load = "EndlessDream.SymMOD";

    switch (__state.song_id) {
        case 1: {
            file_to_load = "electric_drug.mod";
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
}