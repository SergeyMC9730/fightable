
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "fightable/sanitizer.h"
#include "fightable/storage.h"
#include "fightable/string.h"
#include <fightable/sound_engine.h>
#include <fightable/notif_mgr.h>

#include <stdio.h>
#include <stdlib.h>

#include <fraylib.h>

RSB_ARRAY_IMPL_GEN(struct fae_sound_entry, _ae_sound_entry);
RSB_ARRAY_IMPL_GEN(struct fae_sound_cache_entry, _ae_sound_cache_entry);

void _fAudioAccessRaylibEngine(struct faudio_engine* engine) {
    if (!engine->rl_engine.current_sounds) {
        engine->rl_engine.current_sounds = RSBCreateArray_ae_sound_entry();
    }
    if (!engine->rl_engine.sound_cache) {
        engine->rl_engine.sound_cache = RSBCreateArray_ae_sound_cache_entry();
    }
}

void _fAudioUpdateRaylibSounds(struct faudio_engine* engine) {
    if (!engine) return;

    _fAudioAccessRaylibEngine(engine);

    unsigned int i = 0;
    struct fae_sound_entry *entry;
repeat:
    for (i = 0; i < engine->rl_engine.current_sounds->len; i++) {
        entry = engine->rl_engine.current_sounds->objects + i;

        if (!IsSoundPlaying(entry->sound_alias)) {
            if (!entry->loop) {
                TraceLog(LOG_INFO, "Removing sound entry %d (path=%s)", entry->id, entry->path);
                free(entry->path);
                UnloadSoundAlias(entry->sound_alias);
                RSBPopElementAtIndex_ae_sound_entry(engine->rl_engine.current_sounds, i);
                goto repeat;
            } else {
                TraceLog(LOG_INFO, "Playing %s again", entry->path);
                RlPlaySound(entry->sound_alias);

            }
        }
    }
}
unsigned int _fAudioPlayRaylibSound(struct faudio_engine* engine, const char *path, unsigned char in_storage) {
    if (!engine || !path) return 0;

    _fAudioAccessRaylibEngine(engine);

    char *new_path;
    if (!in_storage) {
        new_path = _fCopyString(path);
    } else {
        new_path = (char *)malloc(1024);
        const char *writable = _fStorageGetWritable();
        snprintf(new_path, 1024, "%s/%s", writable, path);
    }

    unsigned int id = 0;

    struct fae_search_result res = _fAudioFindEntryRlByFilename(engine, path);
    if (!res.cache) {
        struct fae_sound_cache_entry cached = {};
        cached.sound = LoadSound(new_path);

        if (IsSoundValid(cached.sound)) {
            cached.filepath = _fCopyString(new_path);
            RSBAddElement_ae_sound_cache_entry(engine->rl_engine.sound_cache, cached);
            res = _fAudioFindEntryRlByFilename(engine, path);
        } else {
            TraceLog(LOG_ERROR, "Cannot load sound %s into cache", new_path);
            free(new_path);
            return id;
        }
    }

    struct fae_sound_entry entry = {};
    entry.id = ++engine->rl_engine.current_id;
    entry.path = _fCopyString(new_path);
    entry.sound_alias = LoadSoundAlias(res.cache->sound);
    entry.cache_data = res.cache;

    id = entry.id;

    if (!IsSoundValid(entry.sound_alias)) {
        TraceLog(LOG_ERROR, "Cannot load sound at %s (path=%s)", entry.path, path);
    } else {
        RlPlaySound(entry.sound_alias);
        RSBAddElement_ae_sound_entry(engine->rl_engine.current_sounds, entry);
    }

    free(new_path);

    return id;
}

void _fAudioSetSoundParamRl(struct faudio_engine* engine, unsigned int sound_id, enum fae_parameter param, float value) {
    if (!engine) return;

    struct fae_search_result res = _fAudioFindEntryRl(engine, sound_id);
    if (!res.alias || !IsSoundValid(res.alias->sound_alias)) {
        char *buffer = (char *)malloc(1024);
        snprintf(buffer, 1024, "%u: <cred,white>invalid sid\n(setting param)", sound_id);
        _fNotifMgrSend(buffer);
        free(buffer);
        return;
    }

    Sound snd = res.alias->sound_alias;

    // TraceLog(LOG_INFO, "Param %d of %s is set to %f", (int)param, res.alias->path, value);

    switch (param) {
        case AE_VOLUME: {
            value = _fSanitizeFloat(value * engine->volume, 0, 2);
            TraceLog(LOG_INFO, "%s: vol=%f cuz engine->volume=%f", __FILE__, value, engine->volume);
            SetSoundVolume(snd, value);
            break;
        }
        case AE_PAN: {
            SetSoundPan(snd, value);
            break;
        }
        case AE_PITCH: {
            SetSoundPitch(snd, value);
            break;
        }
        case AE_LOOP: {
            res.alias->loop = value >= 1.f;
            break;
        }
    }
}
void _fAudioRunSoundActionRl(struct faudio_engine* engine, unsigned int sound_id, enum fae_action act) {
    if (!engine) return;

    struct fae_search_result res = _fAudioFindEntryRl(engine, sound_id);
    if (!res.alias || !IsSoundValid(res.alias->sound_alias)) {
        char *buffer = (char *)malloc(1024);
        snprintf(buffer, 1024, "%u: <cred,white>invalid sid\n(action)", sound_id);
        _fNotifMgrSend(buffer);
        free(buffer);
        return;
    }

    Sound snd = res.alias->sound_alias;

    TraceLog(LOG_INFO, "Running action %d on %s", (int)act, res.alias->path);

    switch (act) {
        case AE_STOP: {
            StopSound(snd);
            break;
        }
        case AE_PLAY: {
            RlPlaySound(snd);
            break;
        }
        case AE_PAUSE: {
            PauseSound(snd);
            break;
        }
        case AE_RESUME: {
            ResumeSound(snd);
            break;
        }
    }
}

struct fae_search_result _fAudioFindEntryRl(struct faudio_engine* engine, unsigned int sound_id) {
    if (!engine || sound_id == 0) return (struct fae_search_result){};

    _fAudioAccessRaylibEngine(engine);

    struct fae_search_result result = {};

    for (unsigned int i = 0; i < engine->rl_engine.current_sounds->len; i++) {
        struct fae_sound_entry *e = engine->rl_engine.current_sounds->objects + i;
        if (e->id == sound_id) {
            result.alias = e;
            result.cache = e->cache_data;
            return result;
        }
    }

    return result;
}
struct fae_search_result _fAudioFindEntryRlByFilename(struct faudio_engine* engine, const char *ending) {
    if (!engine || !ending) return (struct fae_search_result){};

    _fAudioAccessRaylibEngine(engine);

    struct fae_search_result result = {};

    for (unsigned int i = 0; i < engine->rl_engine.current_sounds->len; i++) {
        struct fae_sound_entry *e = engine->rl_engine.current_sounds->objects + i;
        if (_fStringEndsWith(e->path, ending)) {
            result.alias = e;
            result.cache = e->cache_data;
            return result;
        }
    }

    if (!result.cache) {
        for (unsigned int i = 0; i < engine->rl_engine.sound_cache->len; i++) {
            struct fae_sound_cache_entry *e = engine->rl_engine.sound_cache->objects + i;
            if (_fStringEndsWith(e->filepath, ending)) {
                result.cache = e;
                return result;
            }
        }
    }

    return result;
}
