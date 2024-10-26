#include <fightable/sound_engine.h>
#include <libopenmpt/libopenmpt.h>

const char *_fAudioGetDbg(struct faudio_engine *engine) {
    return openmpt_module_format_pattern_row_channel(engine->current_module, engine->_pattern, engine->_row, 0, 0, 0);
}