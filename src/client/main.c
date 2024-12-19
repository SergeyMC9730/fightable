#include <fightable/state.h>
#include <fightable/tilemap.h>
#include <fightable/renderer.h>
#include <fightable/level.h>
#include <stdlib.h>
#include <string.h>
#include <fightable/editor.h>
#include <stdio.h>
#include <fightable/intro.h>
#include <fightable/debug.h>
#ifndef _DISABLE_MP_SERVER_
#include <fightable/http/http_server.h>
#endif
#include <fightable/storage.h>
#include <cJSON.h>
#include <fightable/flags.h>
#include <time.h>
#include <pthread.h>
int flags = 0;
bool v_sync_flag = 1;
struct flevel __level;
struct ftilemap __tilemap;

void *main_thr0(void *user) {
    _fAudioBegin(&__state.sound_engine);

    return 0;
}

#ifdef TARGET_ANDROID

#include <stdarg.h>
#include <android/log.h>
#include <android_native_app_glue.h>

void _fAndroidTraceLog(int level, const char *text, __builtin_va_list args) {
    __android_log_vprint(ANDROID_LOG_VERBOSE, "fightable", text, args);
}
#endif

void _fMainLog(const char *msg) {
    TraceLog(LOG_INFO, msg);
}

#ifdef TARGET_ANDROID
struct android_app *GetAndroidApp();
#endif

struct fresource_file {
    const char *filename;
    unsigned char *data;
    int data_size;
};

void _fMainLoadResource(struct fresource_file *file) {
    file->data = LoadFileData(file->filename, &file->data_size);
}
void _fMainLoadResources(struct fresource_file *resources, int files) {
    for (int i = 0; i < files; i++) {
        struct fresource_file *resource = resources + i;
        _fMainLoadResource(resource);
    }
}

void _fMainCloneResources(struct fresource_file *resources, int files) {
    const char *path = _fStorageGetWritable();
    char *buffer = (char *)MemAlloc(2048);
    
    for (int i = 0; i < files; i++) {
        struct fresource_file resource = resources[i];
        if (!resource.data) {
            TraceLog(LOG_WARNING, "Resource file %s does not contain data", resource.filename);
            continue;
        }
        if (resource.data_size <= 0) {
            TraceLog(LOG_WARNING, "Resource file %s has invalid data size (%d)", resource.filename, resource.data_size);
            continue;
        }

        snprintf(buffer, 2048, "%s/%s", path, resource.filename);
        SaveFileData(buffer, resource.data, resource.data_size);
    }

    MemFree(buffer);
}
void _fMainDestroyResources(struct fresource_file *resources, int files) {
    for (int i = 0; i < files; i++) {
        struct fresource_file resource = resources[i];
        if (!resource.data) {
            TraceLog(LOG_WARNING, "Resource file %s does not contain data, cannot be cleaned up", resource.filename);
            continue;
        }
        if (resource.data_size <= 0) {
            TraceLog(LOG_WARNING, "Resource file %s has invalid data size (%d), cannot be cleaned up", resource.filename, resource.data_size);
            continue;
        }

        MemFree(resource.data);
    }
}

int main(int argc, char **argv) {
#ifdef TARGET_ANDROID
    __state.system = GetAndroidApp();
#endif
    char *dbg_buffer = (char *)MemAlloc(2048);

    Vector2 win_sz = {800, 600};
    Vector2 actual_sz = win_sz;
    Vector2 editor_sz = {255, 0};

    SetRandomSeed(time(0));

    _fStoragePrepareWritable();

    unsigned char debug_output = 1;

#ifdef TARGET_ANDROID
    SetTraceLogCallback(_fAndroidTraceLog);
    actual_sz = (Vector2){0, 0};
#else
    __state.window_scale = UI_SCALE;
    // SetTraceLogLevel(LOG_WARNING | LOG_ERROR);
#endif

    SetAudioStreamBufferSizeDefault(CHANNEL_BUFFER_SIZE);

#ifdef COTARGET_PTX
    _fPtxInit();

    // __state.can_use_gpu_accel = 0;
#endif
    flags = 0;
    v_sync_flag = 1;
    // if(v_sync_flag) flags |= FLAG_VSYNC_HINT; 
    SetConfigFlags(flags);
    InitWindow(actual_sz.x, actual_sz.y, "Fightable");
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));
    // SetTargetFPS(30);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetExitKey(KEY_NULL);

    Vector2 ui_scaling = GetWindowScaleDPI();

    actual_sz.x *= ui_scaling.x;
    actual_sz.y *= ui_scaling.y;

    editor_sz.x *= ui_scaling.x;
    editor_sz.y *= ui_scaling.y;

#ifdef TARGET_ANDROID
    __state.initial_game_size = win_sz;
#else
    __state.initial_game_size = actual_sz;
#endif

    SetWindowSize(actual_sz.x, actual_sz.y);

#ifndef TARGET_ANDROID
    ChangeDirectory("assets");
#endif

    SetWindowIcon(RlLoadImage("icon.png"));

    InitAudioDevice();

    int result = pthread_create(&__state.sound_thread, NULL, main_thr0, NULL);
    TraceLog(LOG_INFO, "pthread_create: result value %d", result);

    struct fresource_file resources[] = {
        {"fightable1.png"},
        {"text.png"}
    };

    _fMainLoadResources(resources, sizeof(resources) / sizeof(struct fresource_file));

    __tilemap = _fTilemapCreate("fightable1.png", (IVector2){8, 8});
    __state.tilemap = &__tilemap;

    __state.text_manager = _fTextLoadDefault();

    __level = _fLevelLoadTest(__state.tilemap, (IVector2){28, 4});
    __state.current_level = &__level;

    if (argc > 1) {
        printf("ARGV[1] = %s\n", argv[1]);

        if (strcmp(argv[1], "editor") == 0) {
            __state.current_editor = _fEditorCreate();
            free(__level.objects);

            __state.current_level = NULL;

            actual_sz.x += editor_sz.x; win_sz.x += editor_sz.x;
            actual_sz.y += editor_sz.y; win_sz.y += editor_sz.y;

#ifndef TARGET_ANDROID
            SetWindowSize(actual_sz.x, actual_sz.y);
#endif
        }
    }

    __state.base_game_size = actual_sz;
    __state.editor_size = editor_sz;

#ifndef DEBUG
    _fIntroInit();
#endif

    RenderTexture2D txt = LoadRenderTexture(win_sz.x / UI_SCALE, win_sz.y / UI_SCALE);
    __state.framebuffer = txt;

    __state.overlay_framebuffer = LoadRenderTexture(actual_sz.x / ui_scaling.x, actual_sz.y / ui_scaling.y);

    unsigned char shake_lock[8] = {0};

#ifndef TARGET_ANDROID
    ChangeDirectory("..");
#endif

#ifndef _DISABLE_MP_SERVER_
    __state.webserver = _fHttpServerCreate(3000, _fMainLog);
#endif

    _fMainCloneResources(resources, sizeof(resources) / sizeof(struct fresource_file));
    _fMainDestroyResources(resources, sizeof(resources) / sizeof(struct fresource_file));

    {
        cJSON *data = _fTilemapCreateJson(__state.tilemap);
        if (data) {
            char *data_str = cJSON_Print(data);

            snprintf(dbg_buffer, 2048, "%s/fightable1.json", _fStorageGetWritable());
            SaveFileText(dbg_buffer, data_str);
        }
    }

    {
        cJSON *data = _fTilemapCreateJson(&__state.text_manager.tilemap);
        if (data) {
            char *data_str = cJSON_Print(data);

            snprintf(dbg_buffer, 2048, "%s/text.json", _fStorageGetWritable());
            SaveFileText(dbg_buffer, data_str);
        }
    }

#ifndef _DISABLE_MP_SERVER_
#ifdef TARGET_ANDROID
    _fHttpSetAllowedResourceDir(__state.webserver, __state.system->activity->internalDataPath);
#else
    _fHttpSetAllowedResourceDir(__state.webserver, _fStorageGetWritable());
#endif
#endif

    __state.gfx.fade_v.should_process = 1;

    _fKeyboardRegister(&__state.kbd, KEY_ESCAPE);
    _fKeyboardRegister(&__state.kbd, KEY_F1);
    _fKeyboardRegister(&__state.kbd, KEY_A);
    _fKeyboardRegister(&__state.kbd, KEY_D);
    _fKeyboardRegister(&__state.kbd, KEY_LEFT);
    _fKeyboardRegister(&__state.kbd, KEY_RIGHT);
    _fKeyboardRegister(&__state.kbd, KEY_SPACE);

    SetTextLineSpacing((int)(15.f / GetWindowScaleDPI().y * 1.5f));

    while (!WindowShouldClose()) {
        actual_sz.x = GetRenderWidth();
        actual_sz.y = GetRenderHeight();

        __state.base_game_size = actual_sz;

        _fGfxUpdate(&__state.gfx);
        __state.gui_render_offset.x = __state.gfx.shake_v.x;
        __state.gui_render_offset.y = __state.gfx.shake_v.y;

        if (IsKeyPressed(KEY_G)) {
            // _fGfxShake(&__state.gfx, 4.f);
            _fGfxFadeInOut(&__state.gfx, BLACK, BLANK, 0.5f);
        }

        BeginDrawing();
        BeginTextureModeStacked(__state.framebuffer);

        _fDraw();

        EndTextureModeStacked();

        BeginTextureModeStacked(__state.overlay_framebuffer);
        ClearBackground(BLANK);
        _fSchedulerIterateOverlays();
        EndTextureModeStacked();

        ClearBackground(BLACK);

        double scaling_y = (double)actual_sz.y / (double)__state.framebuffer.texture.height; 
        int align_x = (actual_sz.x - (__state.framebuffer.texture.width * scaling_y)) / 2;

        __state.mouse_pos_offset = (Vector2){align_x, 0};
        __state.window_scale = scaling_y;

        {
            RLRectangle source = (RLRectangle){ 0, 0, (float)__state.framebuffer.texture.width, (float)-__state.framebuffer.texture.height };
            RLRectangle dest = (RLRectangle){ align_x, 0, (float)__state.framebuffer.texture.width * scaling_y, (float)__state.framebuffer.texture.height * scaling_y };

            DrawTexturePro(__state.framebuffer.texture, source, dest, (Vector2){0, 0}, 0.f, WHITE);
        }

        {
            double scaling_y = (double)actual_sz.y / (double)__state.overlay_framebuffer.texture.height; 
            int align_x = (actual_sz.x - (__state.overlay_framebuffer.texture.width * scaling_y)) / 2;

            RLRectangle source = (RLRectangle){ 0, 0, (float)__state.overlay_framebuffer.texture.width, (float)-__state.overlay_framebuffer.texture.height };
            RLRectangle dest = (RLRectangle){ align_x, 0, (float)__state.overlay_framebuffer.texture.width * scaling_y, (float)__state.overlay_framebuffer.texture.height * scaling_y };

            DrawTexturePro(__state.overlay_framebuffer.texture, source, dest, (Vector2){0, 0}, 0.f, WHITE);
        }

        _fGfxDraw(&__state.gfx);

        DrawFPS(32, 8);

        if (debug_output) {
            snprintf(dbg_buffer, 2048, "   offset: %d\n   ui scale: %f\n   window scale: %f\n   mus time: %f\n   playing: %s\n   song stage: %d\n   song id: %d\n   render area: %d:%d (%d:%d tiles)\n gpu time: %fms", 
                align_x,
                (float)UI_SCALE,
                (float)__state.window_scale,
                (float)_fAudioGetPlayTime(&__state.sound_engine),
                _fAudioGetSongName(&__state.sound_engine),
                __state.title_song_stage,
                (int)__state.song_id,
                __state.framebuffer.texture.width, __state.framebuffer.texture.height,
                __state.framebuffer.texture.width / __state.tilemap->tile_size.x, __state.framebuffer.texture.height / __state.tilemap->tile_size.y,
                __state.cuda_time
            );

            RlDrawText(dbg_buffer, 8, 32, 20, RED);
        }

        EndDrawing();

        __state.frames_rendered++;
        __state.time += (double)GetFrameTime();
    }

    _fTilemapUnload(&__tilemap);
    UnloadRenderTexture(txt);
    
    if (__state.current_level) {
        UnloadTexture(__state.current_level->background_tile);
        free(__state.current_level->objects);
    }

    __state.sound_engine.should_stop = 1;
    pthread_join(__state.sound_thread, NULL);

#ifndef _DISABLE_MP_SERVER_
    if (__state.webserver != NULL) {
        _fHttpServerDestroy(__state.webserver);
    }
#endif

    return 0;
}
