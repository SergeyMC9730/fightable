#include <fightable/sound_engine.h>

void _fAudioStop(struct faudio_engine *engine) {
    _fAudioPlayModule(engine, 0);
}