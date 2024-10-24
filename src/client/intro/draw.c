#include <fightable/intro.h>
#include <raylib.h>
#include <fightable/state.h>
#include <stdio.h>

void _fIntroDraw() {
    ClearBackground(BLACK);

    double t = _fAudioGetPlayTime(&__state.sound_engine);

    char buffer[20] = {};
    snprintf(buffer, 20, "t: %f", (float)t);

    _fTextDraw(&__state.text_manager, buffer, (IVector2){1, 1}, GREEN, 1);

    if (__state.title_song_stage == 0) {

    }

    return;
}