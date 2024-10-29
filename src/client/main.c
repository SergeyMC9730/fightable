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

#ifdef TARGET_ANDROID
#include <android/log.h>
#endif

struct flevel __level;
struct ftilemap __tilemap;

void *main_thr0(void *user) {
    _fAudioBegin(&__state.sound_engine);

    return 0;
}

#ifdef TARGET_ANDROID
void _fAndroidTraceLog(int level, const char *text, __builtin_va_list args) {
    __android_log_vprint(ANDROID_LOG_VERBOSE, "fightable", text, args);
}
#endif

int main(int argc, char **argv) {
    Vector2 win_sz = {800, 600};
    Vector2 actual_sz = win_sz;
    Vector2 editor_sz = {255, 0};

#ifdef TARGET_ANDROID
    SetTraceLogCallback(_fAndroidTraceLog);
    actual_sz = (Vector2){0, 0};
#endif

    // SetTraceLogLevel(LOG_WARNING | LOG_ERROR);

    SetAudioStreamBufferSizeDefault(CHANNEL_BUFFER_SIZE);

    InitWindow(actual_sz.x, actual_sz.y, "Fightable");
    SetTargetFPS(60);
    SetWindowState(FLAG_WINDOW_RESIZABLE);

    Vector2 ui_scaling = GetWindowScaleDPI();
    actual_sz.x *= ui_scaling.x;
    actual_sz.y *= ui_scaling.y;

    SetWindowSize(actual_sz.x, actual_sz.y);

    ChangeDirectory("assets");

    SetWindowIcon(LoadImage("icon.png"));

    InitAudioDevice();

    int result = pthread_create(&__state.sound_thread, NULL, main_thr0, NULL);
    TraceLog(LOG_INFO, "pthread_create: result value %d", result);

// #ifdef TARGET_ANDROID
//     while (!__state.sound_engine.stream);

//     PlayAudioStream(*__state.sound_engine.stream);
// #endif

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

            win_sz.x += editor_sz.x;
            SetWindowSize(win_sz.x, win_sz.y);
            SetWindowTitle("Fightable Editor");
        }
    }

#ifndef DEBUG
    _fIntroInit();
#endif

    RenderTexture2D txt = LoadRenderTexture(win_sz.x / UI_SCALE, win_sz.y / UI_SCALE);
    __state.framebuffer = txt;

    char *dbg_buffer = (char *)MemAlloc(2048);

    unsigned char shake_lock = 0;

    ChangeDirectory("..");

    while (!WindowShouldClose()) {
        actual_sz.x = GetRenderWidth();
        actual_sz.y = GetRenderHeight();

        _fGfxUpdate(&__state.gfx);
        __state.gui_render_offset.x = __state.gfx.shake_v.x;
        __state.gui_render_offset.y = __state.gfx.shake_v.y;

        if (IsKeyPressed(KEY_G)) {
            _fGfxShake(&__state.gfx, 4.f);
        }

        BeginDrawing();
        BeginTextureModeStacked(__state.framebuffer);

        _fDraw();

        EndTextureModeStacked();

        ClearBackground(BLACK);

        double scaling_y = (double)actual_sz.y / (double)__state.framebuffer.texture.height; 
        int align_x = (actual_sz.x - (__state.framebuffer.texture.width * scaling_y)) / 2;

        __state.mouse_pos_offset = (Vector2){align_x, 0};
        __state.window_scale = scaling_y;

        Rectangle source = (Rectangle){ 0, 0, (float)__state.framebuffer.texture.width, (float)-__state.framebuffer.texture.height };
        Rectangle dest = (Rectangle){ align_x, 0, (float)__state.framebuffer.texture.width * scaling_y, (float)__state.framebuffer.texture.height * scaling_y };

        DrawTexturePro(__state.framebuffer.texture, source, dest, (Vector2){0, 0}, 0.f, WHITE);

        DrawFPS(32, 8);

        const char *row = _fAudioGetDbg(&__state.sound_engine, 5);

        snprintf(dbg_buffer, 2048, "   offset: %d\n   ui scale: %f\n   window scale: %f", 
            align_x,
            (float)UI_SCALE,
            (float)__state.window_scale
        );

        DrawText(dbg_buffer, 8, 32, 20, RED);

        if (row != NULL) {
            if (strstr(row, "0D") != NULL || strstr(row, "14") != NULL) {
                if (!shake_lock) {
                    // printf("shaking\n");

                    _fGfxShake(&__state.gfx, 1.f);
                    shake_lock = 1;
                }
            } else {
                shake_lock = 0;
            }

            free(row);
        }

        Vector2 mpos = GetMousePosition();

        DrawRectangle(mpos.x, mpos.y, 12, 12, RED);

        // DrawText(dbg_buffer, 8, 8, 20.f, YELLOW);

        EndDrawing();

        __state.frames_rendered++;
        __state.time += (double)GetFrameTime();
    }

    _fTilemapUnload(__tilemap);
    UnloadRenderTexture(txt);
    
    if (__state.current_level) {
        UnloadTexture(__state.current_level->background_tile);
        free(__state.current_level->objects);
    }

    __state.sound_engine.should_stop = 1;
    pthread_join(__state.sound_thread, NULL);

    return 0;
}
