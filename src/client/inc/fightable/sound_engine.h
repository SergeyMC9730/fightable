
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#ifdef IAUDIO_ENGINE
// do not change these values if you dont know what are you doing

#define CHANNEL_BUFFER_SIZE 480
#define CHANNEL_SAMPLE_RATE 48000
#endif

typedef struct openmpt_module openmpt_module;
typedef struct AudioStream AudioStream;

#include <fraylib.h>
#include <rsb/rsb_array_gen.h>

struct fae_sound_cache_entry {
    Sound sound;
    char *filepath;
};
struct fae_sound_entry {
    Sound sound_alias;
    char *path;
    unsigned int id;
    unsigned char loop;
    struct fae_sound_cache_entry *cache_data;
};

struct fae_search_result {
    struct fae_sound_cache_entry *cache;
    struct fae_sound_entry *alias;
};

RSB_ARRAY_DEF_GEN(struct fae_sound_entry, _ae_sound_entry);
RSB_ARRAY_DEF_GEN(struct fae_sound_cache_entry, _ae_sound_cache_entry);

struct faudio_engine {
    openmpt_module *current_module;

    short *buffer;

    unsigned char mod_lock : 1;
    unsigned char should_stop : 1;
    unsigned char ready : 1;
    unsigned char do_not_shake : 1;

    unsigned int _order;
    unsigned int _pattern;
    unsigned int _row;
    unsigned int _channels;

#if defined(TARGET_ANDROID) || defined (_DISABLE_PORTAUDIO_)
    AudioStream *stream;
#endif

    // volume allows values from 0 to 2
    float volume;

    struct _fx {
        unsigned char perform_volume_slider : 1;

        float slide_volume_old;
        float slide_volume_value;
        float slide_volume_time;
        float slide_volume_cur_time;

        float resulting_volume;

        double time;
    } fx;

    struct rl_snd {
        rsb_array__ae_sound_entry *current_sounds;
        rsb_array__ae_sound_cache_entry *sound_cache;
        unsigned int current_id;
    } rl_engine;
};

enum fae_parameter {
    AE_VOLUME, AE_PAN, AE_PITCH, AE_LOOP
};
enum fae_action {
    AE_STOP, AE_PLAY, AE_PAUSE, AE_RESUME
};

void _fAudioBegin(struct faudio_engine *engine);
unsigned char _fAudioPlayModule(struct faudio_engine *engine, const char *path);
void _fAudioStop(struct faudio_engine *engine);
double _fAudioGetPlayTime(struct faudio_engine *engine);
void _fAudioLoopCurrent(struct faudio_engine *engine);
const char *_fAudioGetChannelRow(struct faudio_engine *engine, int channel);
int _fAudioGetChannelsTotal(struct faudio_engine *engine);
const char *_fAudioGetSongName(struct faudio_engine *engine);
float _fAudioGetVolume(struct faudio_engine *engine);
void _fAudioSetVolume(struct faudio_engine *engine, float v);

void _fAudioFxUpdate(struct faudio_engine* engine);
void _fAudioFxSlideVolume(struct faudio_engine* engine, float v, float time);

void _fAudioUpdateRaylibSounds(struct faudio_engine* engine);
unsigned int _fAudioPlayRaylibSound(struct faudio_engine* engine, const char *path, unsigned char in_storage);

void _fAudioSetSoundParamRl(struct faudio_engine* engine, unsigned int sound_id, enum fae_parameter param, float value);
void _fAudioRunSoundActionRl(struct faudio_engine* engine, unsigned int sound_id, enum fae_action act);

void _fAudioToggleSoundLoopRl(struct faudio_engine* engine, unsigned int sound_id, unsigned char state);

struct fae_search_result _fAudioFindEntryRl(struct faudio_engine* engine, unsigned int sound_id);
struct fae_search_result _fAudioFindEntryRlByFilename(struct faudio_engine* engine, const char *ending);

#ifdef __cplusplus
}
#endif
