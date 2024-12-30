#pragma once

#include <raylib.h>

#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

struct ftilemap;
struct flevel;
struct feditor;
struct renderer_animation;
typedef struct openmpt_module openmpt_module;
#ifndef _DISABLE_MP_SERVER_
struct fhttpserver;
#endif

#define IAUDIO_ENGINE

#include <fightable/text.h>
#include <fightable/sound_engine.h>
#include <fightable/gfx.h>
#include <fightable/keyboard.h>
#include <fightable/button.h>
#include <fightable/config.h>
#include <nt5emul/renderer_animation.h>
#ifndef _DISABLE_MP_SERVER_
#include <fightable/tcpsrv/daemon.h>
#include <fightable/tcpsrv/delegate.h>
#endif
#include <fightable/tcpcln/client.h>
#include <fightable/tcpcln/delegate.h>
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

#define UI_MENU_MAIN        0
#define UI_MENU_MPCREATE    1

#ifdef TARGET_ANDROID
struct android_app;
#endif
struct fightable_state {
    struct ftilemap *tilemap;

    struct flevel *current_level;

    unsigned long frames_rendered;
    long double time;

    RenderTexture2D framebuffer;

    struct feditor *current_editor;
    struct ftitle_editor *current_title_editor;

    struct ftext_manager text_manager;

    // Music title_song;
    struct renderer_animation *title_song_sync;
    int title_song_stage;
    float title_a;
    unsigned char title_r0;

    unsigned char intro_can_continue;

    pthread_t sound_thread;

    struct faudio_engine sound_engine;

    Texture2D raylib_logo;

    Texture2D intro_temp_bg;
    double intro_bg_offsetf;
    int intro_bg_offseti;

    Vector2 gui_render_offset;

    struct gfx_manager gfx;

#define R2D_STACK_SIZE 24
    // rendertexture stack
    RenderTexture2D r2dstack[R2D_STACK_SIZE];
    // pointer to the `r2dstack` end
    int r2dpointer;

    Texture2D menu_borders;

    unsigned char intro_stage_completed;
    unsigned char menu_state;
    unsigned char menu_block_ui;

    Texture2D playbtn_container;

    Vector2 mouse_pos_offset;
    double window_scale;
    
    Vector2 base_game_size;
    Vector2 editor_size;
    Vector2 initial_game_size;

    short song_id;

#ifndef _DISABLE_MP_SERVER_
    struct fhttpserver *webserver;
#endif

#ifdef TARGET_ANDROID
    struct android_app *system;
#endif

    float time1;
    unsigned char menu_perform_move;
    float menu_cur_x;
    
    struct fkeyboard_mgr kbd;

    RenderTexture2D overlay_framebuffer;

    unsigned char can_use_gpu_accel;
    double cuda_time;

    struct fconfig config;

    Texture2D damage_overlay;
    struct renderer_animation* damage_overlay_anim;
    float damage_overlay_timer;
    float damage_overlay_timer2;
    unsigned char damage_overlay_play;

    unsigned char current_ui_menu;

    unsigned char show_debug_info;

#ifndef _DISABLE_MP_SERVER_
    Texture2D mp_create_bg1;
    Texture2D mp_create_bg2;
    Shader mp_create_wave_shader;

    long double mp_create_time;

    struct ftcp_server_daemon* mp_server_instance;
    struct ftcp_server_delegate mp_server_delegate;
    unsigned short mp_server_port;
#endif

    struct ftcpclient* mp_client;
    struct ftcpclient_delegate* mp_client_delegate;

    Vector2 mp_level_preview_offset;
};

extern struct fightable_state __state;

#ifdef __cplusplus
}
#endif