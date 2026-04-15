
//          Sergei Baigerov 2024 - 2026.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "fightable/player_connection.h"
#include <fraylib.h>

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

#endif

#define IAUDIO_ENGINE

#include <fightable/text.h>
#include <fightable/sound_engine.h>
#include <fightable/gfx.h>
#include <fightable/keyboard.h>
#include <fightable/button.h>
#include <fightable/config.h>
#include <nt5emul/renderer_animation.h>
#include <fightable/text_input.h>
#include <fightable/a_clippy.h>
#ifndef _DISABLE_MP_SERVER_
#include <nbnet.h>
#endif
#include <nt5emul/tui/file_selector.h>
#include <fightable/multiline_text_instance.h>
#include <fightable/platform_ui.h>
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
#define UI_MENU_EDITOR      2
#define UI_MENU_MPJOIN      3
#define UI_MENU_SPJOIN      4

typedef struct rsb_array__fnotif_mgr_entry rsb_array__fnotif_mgr_entry;

#ifdef TARGET_ANDROID
struct android_app;
#endif

struct fosk_row;

struct fmp_metadata_req;

struct fightable_state {
    struct ftilemap *tilemap;
    struct ftilemap *tilemap2;

    struct flevel *current_level;

    unsigned long long frames_rendered;
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
    Color intro_text_tint;

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
    unsigned char mp_lobby_bg_ready;
    float mp_lobby_bg_opacity;
    RenderTexture2D mp_lobby_bg;
    unsigned char mp_lobby_lp_moving;

    long double mp_create_time;

    unsigned short mp_server_port;
    unsigned short mp_server_http_port;

    unsigned int mp_server_max_connections;

    unsigned char mp_server_ready;
    unsigned char mp_server_should_tick;
#endif

    Vector2 mp_level_preview_offset;

    float cf_level_x;
    float cf_timer;
    unsigned char cf_prepared;
    unsigned char cf_prepared_2;

    // camera2d stack
    Camera2D c2dstack[R2D_STACK_SIZE];
    // pointer to the `r2dstack` end
    int c2dpointer;

    struct nt_file_selector_menu *current_search_menu;
    char *osk_buffer;
    unsigned int osk_buffer_size;
    struct renderer_animation osk_current_animation;
    RLRectangle osk_input_field;
    renderer_event_t osk_callback;
    struct fosk_row *osk_rows;
    unsigned int osk_rows_size;

    Texture2D test_midground;
    unsigned char display_test_midground;

    rsb_array__fnotif_mgr_entry *notifications;

    RLFont unifont16;
    RLFont ubuntu_mono64;

    struct ftext_input *ip_input;
    struct ftext_input *name_input;

#ifndef _DISABLE_MP_SERVER_
    float mp_client_connect_state;
    unsigned char mp_client_connecting;
    unsigned char mp_client_should_tick;
    char *mp_client_ip;
    unsigned short mp_client_port;
    unsigned short mp_client_http_port;
    struct fnotif_mgr_entry *mp_client_notif_status;
    struct flevel *mp_client_level;
    int mp_client_user_id;

    rsb_array__PlayerCon *mp_connected_players;
#endif

    struct fa_clippy clippy;

    unsigned char sp_opened_level;

    float world_tick_time;
    float world_tick_time_ext;

    enum fplatform_ui ui_mode;
};

extern struct fightable_state __state;

#ifdef __cplusplus
}
#endif
