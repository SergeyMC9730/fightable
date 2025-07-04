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
};

void _fAudioBegin(struct faudio_engine *engine);
unsigned char _fAudioPlayModule(struct faudio_engine *engine, const char *path);
void _fAudioStop(struct faudio_engine *engine);
double _fAudioGetPlayTime(struct faudio_engine *engine);
void _fAudioLoopCurrent(struct faudio_engine *engine);
const char *_fAudioGetDbg(struct faudio_engine *engine, int channel);
int _fAudioGetChannelsTotal(struct faudio_engine *engine);
const char *_fAudioGetSongName(struct faudio_engine *engine);
float _fAudioGetVolume(struct faudio_engine *engine);
void _fAudioSetVolume(struct faudio_engine *engine, float v);

void _fAudioFxUpdate(struct faudio_engine* engine);
void _fAudioFxSlideVolume(struct faudio_engine* engine, float v, float time);
void _fAudioFxInitEcho(struct faudio_engine* engine);

#ifdef __cplusplus
}
#endif
