#include <fightable/intro.h>
#include <fightable/state.h>
#include <stdio.h>

void _fIntroInit() {
    _fAudioPlayModule(&__state.sound_engine, "assets/groo_-_phoenix.mod");
    _fAudioLoopCurrent(&__state.sound_engine);

    __state.raylib_logo = LoadTexture("assets/raylib_16x16.png");
}