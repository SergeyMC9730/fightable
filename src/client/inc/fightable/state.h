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
struct ftitle_editor;

#include <fightable/text.h>
#include <fightable/sound_engine.h>
#include <fightable/editor_library.h>
#include <fightable/gfx.h>

#if _WIN32
    #define NOUSER
    #define NOGDI

    #include <windows.h>
    
    #undef far
    #undef near
    #undef min
    #undef max
#else // unix
    #include <pthread.h>
#endif

struct fightable_state {
    struct ftilemap *tilemap;

    struct flevel *current_level;

    unsigned long frames_rendered;
    double time;

    RenderTexture2D framebuffer;

    struct feditor *current_editor;
    struct ftitle_editor *current_title_editor;

    struct ftext_manager text_manager;

    unsigned long draw_calls;

    // Music title_song;
    struct renderer_animation *title_song_sync;
    int title_song_stage;
    float title_a;
    unsigned char title_r0;

    unsigned char intro_can_continue;

#ifdef _WIN32
    HANDLE sound_thread;
#else
    pthread_t sound_thread;
#endif
    struct faudio_engine sound_engine;

    Texture2D raylib_logo;

    Texture2D intro_temp_bg;
    double intro_bg_offsetf;
    int intro_bg_offseti;

    enum fightable_editor selected_editor_type;

    Vector2 gui_render_offset;

    struct gfx_manager gfx;

#define R2D_STACK_SIZE 32
    // rendertexture stack
    RenderTexture2D r2dstack[R2D_STACK_SIZE];
    // pointer to the `r2dstack` end
    int r2dpointer;

    Texture2D menu_borders;

    unsigned char intro_stage_completed;
    
    unsigned char menu_pressed_play;

    Texture2D playbtn_container;
};

extern struct fightable_state __state;

#ifdef __cplusplus
}
#endif
