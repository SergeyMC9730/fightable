
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#define IAUDIO_ENGINE
#include <fightable/sound_engine.h>

#if !defined(TARGET_ANDROID) && !defined(_DISABLE_PORTAUDIO_)
#pragma message("enabling portaudio")
#include <portaudio.h>
#endif

#include <libopenmpt/libopenmpt.h>
#include <libopenmpt/libopenmpt_stream_callbacks_file.h>

#include <stdlib.h>
#include <string.h>

#include <fraylib.h>

#include <fightable/time.h>

#if !defined(TARGET_ANDROID) && !defined(_DISABLE_PORTAUDIO_)
#define USE_PORTAUDIO
#endif

void _fAudioBegin(struct faudio_engine *engine) {
    int channels = 1;
#ifndef TARGET_ANDROID
    channels = 2;
#endif

    size_t bufsize1 = sizeof(short) * CHANNEL_BUFFER_SIZE * channels;

    engine->buffer = (short *)malloc(bufsize1);

#ifdef USE_PORTAUDIO
    PaStream *stream;

    PaError result = Pa_Initialize();
    if (result != paNoError) {
        TraceLog(LOG_ERROR, "PortAudio failed to initialize itself: %d, %s", (int)result, Pa_GetErrorText(result));

        return;
    }

    result = Pa_OpenDefaultStream(&stream, 0, 2, paInt16 | paNonInterleaved, CHANNEL_SAMPLE_RATE, paFramesPerBufferUnspecified, NULL, NULL);
    if (result != paNoError) {
        TraceLog(LOG_ERROR, "PortAudio failed to open device: %d, %s", (int)result, Pa_GetErrorText(result));

        return;
    }

    result = Pa_StartStream(stream);
    if (result != paNoError) {
        TraceLog(LOG_ERROR, "PortAudio failed to start stream: %d, %s", (int)result, Pa_GetErrorText(result));

        return;
    }

    TraceLog(LOG_INFO, "Initialized PortAudio");
#else
    AudioStream stream = LoadAudioStream(CHANNEL_SAMPLE_RATE, 16, channels);
    PlayAudioStream(stream);

    engine->stream = &stream;

    TraceLog(LOG_INFO, "Initialized audio stream (PortAudio is not available)");
#endif

    engine->ready = 1;

    short *merge_buffer[2] = {engine->buffer, engine->buffer + CHANNEL_BUFFER_SIZE};

    while (1) {
#if !defined(TARGET_ANDROID) && defined(_DISABLE_PORTAUDIO_)
        while (!IsAudioStreamProcessed(stream)) {}
#endif

        if (engine->should_stop) {
            // printf("1\n");
            break;
        }

        if (engine->mod_lock || !engine->current_module) {
            // printf("2\n");
            _fSleep(33);
            continue;
        }

        memset(engine->buffer, 0, bufsize1);

#ifndef TARGET_ANDROID
        float limit_volume = 0.06f;
        unsigned long count = openmpt_module_read_stereo(engine->current_module, CHANNEL_SAMPLE_RATE, CHANNEL_BUFFER_SIZE, merge_buffer[0], merge_buffer[1]);
#else
        float limit_volume = 0.3f;
        while (!IsAudioStreamProcessed(stream));
        unsigned long count = openmpt_module_read_mono(engine->current_module, CHANNEL_SAMPLE_RATE, CHANNEL_BUFFER_SIZE, merge_buffer[0]);
#endif

        float base_volume = _fAudioGetVolume(engine);

        for (int i = 0; i < (bufsize1 / sizeof(short)); i++) {
            engine->buffer[i] = (float)engine->buffer[i] * limit_volume * base_volume;
        }

        engine->_channels = openmpt_module_get_current_playing_channels(engine->current_module);
        engine->_order = openmpt_module_get_current_order(engine->current_module);
        engine->_pattern = openmpt_module_get_current_pattern(engine->current_module);
        engine->_row = openmpt_module_get_current_row(engine->current_module);

        double time = _fAudioGetPlayTime(engine);
        double max = openmpt_module_get_duration_seconds(engine->current_module);

        // TraceLog(LOG_DEBUG, "MPT info: c%d; o%d; p%d; r%d... time: %f/%f", engine->_channels, engine->_order, engine->_pattern, engine->_row, (float)time, (float)max);

        if (openmpt_module_get_repeat_count(engine->current_module) != 0 && time > max) {
            TraceLog(LOG_INFO, "Resetting song");

            openmpt_module_set_position_seconds(engine->current_module, 0);
        }

        // double s = openmpt_module_get_position_seconds(engine->current_module);
        // printf("s=%f\n", (float)s);

        if (count == 0) {
            engine->mod_lock = 1;
            openmpt_module_destroy(engine->current_module);
            engine->current_module = 0;
            engine->mod_lock = 0;

            // printf("3\n");

            continue;
        }

#ifdef USE_PORTAUDIO
        result = Pa_WriteStream(stream, merge_buffer, count);
        if (result != paNoError) {
            TraceLog(LOG_ERROR, "Portaudio failed to write stream: %d, %s", (int)result, Pa_GetErrorText(result));

            continue;
        }
#else
#ifdef TARGET_ANDROID
        UpdateAudioStream(stream, merge_buffer[0], count);
#else
        UpdateAudioStream(stream, merge_buffer[0], count * channels);
#endif
#endif
    }

    TraceLog(LOG_INFO, "Closing sound engine");

#ifdef USE_PORTAUDIO
    Pa_StopStream(stream);
    Pa_CloseStream(stream);
    Pa_Terminate();
#else
    StopAudioStream(stream);
    UnloadAudioStream(stream);
#endif

    if (engine->current_module) {
        engine->mod_lock = 1;
        openmpt_module_destroy(engine->current_module);
        engine->current_module = 0;
        engine->mod_lock = 0;
    }
}
