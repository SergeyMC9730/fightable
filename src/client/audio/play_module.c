#include <fightable/sound_engine.h>
#include <libopenmpt/libopenmpt.h>
#include <libopenmpt/libopenmpt_stream_callbacks_file.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void _fIntroMusLog(const char *message, void *user) {
    printf("DEBUG: LIBOPENMPT: %s\n", message);
}
int _fIntroMusError(int error, void *user) {
    printf("ERROR: LIBOPENMPT: error %d\n", error);

    return 0;
}

void _fAudioPlayModule(struct faudio_engine *engine, const char *path) {
    if (engine->current_module) {
        engine->mod_lock = 1;
        openmpt_module_destroy(engine->current_module);
        engine->current_module = 0;
        engine->mod_lock = 0;
    }

    if (!path) return;

    int error = 0;
    const char *error_desc = NULL;

    FILE *mod_file = fopen(path, "rb");

    if (!mod_file) {
        printf("file %s cannot be found", path);

        return;
    }

    engine->mod_lock = 1;
    openmpt_module *module = openmpt_module_create2(openmpt_stream_get_file_callbacks2(), mod_file, _fIntroMusLog, NULL, _fIntroMusError, NULL, NULL, NULL, NULL);

    openmpt_module_set_position_seconds(module, 0);

    if (error != 0) {
        printf("failure on opening mod file: %s (%d)", error_desc, error);

        engine->mod_lock = 0;
        return;
    }

    fclose(mod_file);

    printf("opened module %s\n", path);

    engine->current_module = module;
    engine->mod_lock = 0;
}