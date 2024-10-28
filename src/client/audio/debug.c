#include <fightable/sound_engine.h>
#include <libopenmpt/libopenmpt.h>

const char *_fAudioGetDbg(struct faudio_engine *engine, int channel) {
    if (!engine->current_module) return NULL;

    return openmpt_module_format_pattern_row_channel(engine->current_module, engine->_pattern, engine->_row, channel, 0, 0);
}