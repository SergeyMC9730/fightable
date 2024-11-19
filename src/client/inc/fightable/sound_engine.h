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

    unsigned char mod_lock;
    unsigned char should_stop;
    unsigned char ready;

    unsigned int _order;
    unsigned int _pattern;
    unsigned int _row;
    unsigned int _channels;

#ifdef TARGET_ANDROID
    AudioStream *stream;
#endif

    // VOLUME SHOULD BE FROM 0 TO 2
    float volume;
};

void _fAudioBegin(struct faudio_engine *engine);
void _fAudioPlayModule(struct faudio_engine *engine, const char *path);
void _fAudioStop(struct faudio_engine *engine);
double _fAudioGetPlayTime(struct faudio_engine *engine);
void _fAudioLoopCurrent(struct faudio_engine *engine);
const char *_fAudioGetDbg(struct faudio_engine *engine, int channel);
int _fAudioGetChannelsTotal(struct faudio_engine *engine);
const char *_fAudioGetSongName(struct faudio_engine *engine);
float _fAudioGetVolume(struct faudio_engine *engine);
void _fAudioSetVolume(struct faudio_engine *engine, float v);

#ifdef __cplusplus
}
#endif