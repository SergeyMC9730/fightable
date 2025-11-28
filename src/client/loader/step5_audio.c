#include <fraylib.h>
#define IAUDIO_ENGINE
#include <fightable/sound_engine.h>
#include <fightable/pthread_compat.h>
#include <fightable/state.h>

void *_fLoaderMainPrepareAudioThread(void *user) {
    TraceLog(LOG_INFO, "[LOADER] Initializing game's audio subsystem");
    _fAudioBegin(user);
    return 0;
}

void _fLoaderMainPrepareAudio() {
    TraceLog(LOG_INFO, "[LOADER] Initializing audio device");
    SetAudioStreamBufferSizeDefault(CHANNEL_BUFFER_SIZE);
    InitAudioDevice();

    int result = pthread_create(&__state.sound_thread, NULL, _fLoaderMainPrepareAudioThread, &__state.sound_engine);
    TraceLog(LOG_INFO, "pthread_create: result value %d", result);
}
