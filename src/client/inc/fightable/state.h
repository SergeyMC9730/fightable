#pragma once

#include <raylib.h>

#ifdef __cplusplus
extern "C" {
#endif

struct ftilemap;
struct flevel;
struct feditor;
struct renderer_animation;
typedef struct openmpt_module openmpt_module;

#include <fightable/text.h>
#include <fightable/sound_engine.h>
#include <pthread.h>

struct fightable_state {
    struct ftilemap *tilemap;

    struct flevel *current_level;

    unsigned long frames_rendered;
    double time;

    RenderTexture2D framebuffer;

    struct feditor *current_editor;

    struct ftext_manager text_manager;

    unsigned long draw_calls;

    // Music title_song;
    struct renderer_animation *title_song_sync;
    int title_song_stage;

    unsigned char intro_can_continue;

    pthread_t sound_thread;
    struct faudio_engine sound_engine;
};

extern struct fightable_state __state;

#ifdef __cplusplus
}
#endif
