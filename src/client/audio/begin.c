#define IAUDIO_ENGINE
#include <fightable/sound_engine.h>

#include <portaudio.h>

#include <libopenmpt/libopenmpt.h>
#include <libopenmpt/libopenmpt_stream_callbacks_file.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void _fAudioBegin(struct faudio_engine *engine) {
    size_t bufsize1 = sizeof(short) * CHANNEL_BUFFER_SIZE * 2;

    engine->buffer = (short *)malloc(bufsize1);

    PaStream *stream;

    Pa_Initialize();
    Pa_OpenDefaultStream(&stream, 0, 2, paInt16 | paNonInterleaved, CHANNEL_SAMPLE_RATE, paFramesPerBufferUnspecified, NULL, NULL);
    Pa_StartStream(stream);

    engine->ready = 1;

    short *merge_buffer[2] = {engine->buffer, engine->buffer + CHANNEL_BUFFER_SIZE};

    while (1) {
        if (engine->should_stop) {
            // printf("1\n");
            break;
        }

        if (engine->mod_lock || !engine->current_module) {
            // printf("2\n");
            Pa_Sleep((1.f / 30.f) * 1000.f);
            continue;
        }

        // memset(engine->buffer, 0, bufsize1);

        unsigned long count = openmpt_module_read_stereo(engine->current_module, CHANNEL_SAMPLE_RATE, CHANNEL_BUFFER_SIZE, merge_buffer[0], merge_buffer[1]);

        for (int i = 0; i < (bufsize1 / sizeof(short)); i++) {
            engine->buffer[i] = (float)engine->buffer[i] * 0.06f;
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

        Pa_WriteStream(stream, merge_buffer, count);
    }

    printf("closing sound engine\n");

    Pa_StopStream(stream);
    Pa_CloseStream(stream);
    Pa_Terminate();

    if (engine->current_module) {
        engine->mod_lock = 1;
        openmpt_module_destroy(engine->current_module);
        engine->current_module = 0;
        engine->mod_lock = 0;
    }
}