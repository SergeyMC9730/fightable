
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/sound_engine.h>
#include <libopenmpt/libopenmpt.h>
#include <libopenmpt/libopenmpt_stream_callbacks_file.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fraylib.h>

void _fIntroMusLog(const char *message, void *user) {
    // printf("DEBUG: LIBOPENMPT: %s\n", message);
    TraceLog(LOG_INFO, "libopenmpt: %s", message);
}
int _fIntroMusError(int error, void *user) {
    TraceLog(LOG_ERROR, "libopenmpt: error %d", error);

    return 0;
}

unsigned char _fAudioPlayModule(struct faudio_engine *engine, const char *path) {
    if (engine->current_module) {
        engine->mod_lock = 1;
        openmpt_module_destroy(engine->current_module);
        engine->current_module = 0;
        engine->mod_lock = 0;
    }

    if (!path) return 0;

    int error = 0;
    const char *error_desc = NULL;

    int sz = 0;
    unsigned char *data = LoadFileData(path, &sz);

    if (!data || sz == 0) {
        TraceLog(LOG_ERROR, "File %s could not be found", path);

        return 0;
    }

    engine->mod_lock = 1;
    openmpt_module *module = openmpt_module_create_from_memory2(data, sz, _fIntroMusLog, 0, _fIntroMusError, 0, &error, &error_desc, 0);

    if (error != 0 || !module) {
        TraceLog(LOG_ERROR, "Failure on opening mod file: %s (%d)", error_desc, error);

        engine->mod_lock = 0;

        MemFree(data);

        return 0;
    }

    openmpt_module_set_position_seconds(module, 0);

    TraceLog(LOG_INFO, "Opened mod file %s", path);

    engine->current_module = module;
    engine->mod_lock = 0;

    return 1;
}
