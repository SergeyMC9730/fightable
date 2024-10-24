#include <fightable/sound_engine.h>
#include <libopenmpt/libopenmpt.h>

double _fAudioGetPlayTime(struct faudio_engine *engine) {
    if (!engine->current_module) return 0;

    return openmpt_module_get_position_seconds(engine->current_module);
}