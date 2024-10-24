#include <fightable/sound_engine.h>
#include <libopenmpt/libopenmpt.h>

void _fAudioLoopCurrent(struct faudio_engine *engine) {
    if (!engine->current_module) return;
    
    openmpt_module_set_repeat_count(engine->current_module, 0xFFFFFF);
}