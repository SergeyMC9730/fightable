#include <fightable/sound_engine.h>
#include <libopenmpt/libopenmpt.h>

const char *_fAudioGetSongName(struct faudio_engine *engine) {
    if (!engine->current_module) return "none";

    return openmpt_module_get_metadata(engine->current_module, "title");
}