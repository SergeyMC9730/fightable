#include <fightable/sound_engine.h>
#include <libopenmpt/libopenmpt.h>

int _fAudioGetChannelsTotal(struct faudio_engine *engine) {
    return openmpt_module_get_num_channels(engine->current_module);
}