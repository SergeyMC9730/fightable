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
#include <fightable/editor_library.h>

struct flevel __level;
struct ftilemap __tilemap;

void *main_thr0(void *user) {
    _fAudioBegin(&__state.sound_engine);
}

int main(int argc, char **argv) {
    pthread_create(&__state.sound_thread, NULL, main_thr0, NULL);

    Vector2 win_sz = {800, 600};
    Vector2 editor_sz = {255, 0};

    SetTraceLogLevel(LOG_WARNING | LOG_ERROR);

    InitWindow(win_sz.x, win_sz.y, "Fightable");
    SetTargetFPS(144);

    SetWindowIcon(LoadImage("assets/icon.png"));

    InitAudioDevice();

    __tilemap = _fTilemapCreate("assets/fightable1.png", (IVector2){8, 8});
    __state.tilemap = &__tilemap;

    __state.text_manager = _fTextLoadDefault();

    __level = _fLevelLoadTest(__state.tilemap, (IVector2){28, 4});
    __state.current_level = &__level;

    if (argc > 1) {
        printf("ARGV[1] = %s\n", argv[1]);

        if (strcmp(argv[1], "editor") == 0) {
            enum fightable_editor selected_editor = EditorMap;

            if (argc <= 2) {
                selected_editor = EditorMap;
            } else {
                if (strcmp(argv[2], "map") == 0) {
                    selected_editor = EditorMap;
                } else if (strcmp(argv[2], "title") == 0) {
                    selected_editor = EditorTitle;
                } else {
                    printf("unknown editor has been selected\n");

                    __state.sound_engine.should_stop = 1;

                    CloseWindow();
                    pthread_join(__state.sound_thread, NULL);

                    return 1;
                }
            }

            __state.selected_editor_type = selected_editor;

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

    RenderTexture2D txt = LoadRenderTexture(win_sz.x / 5, win_sz.y / 5);
    __state.framebuffer = txt;

    char *dbg_buffer = (char *)MemAlloc(2048);

    unsigned char shake_lock = 0;

    while (!WindowShouldClose()) {
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

        Rectangle source = (Rectangle){ 0, 0, (float)__state.framebuffer.texture.width, (float)-__state.framebuffer.texture.height };
        Rectangle dest = (Rectangle){ 0, 0, GetRenderWidth(), GetRenderHeight() };

        DrawTexturePro(__state.framebuffer.texture, source, dest, (Vector2){0, 0}, 0.f, WHITE);

        DrawFPS(8, 8);

        const char *row = _fAudioGetDbg(&__state.sound_engine, 5);

        // snprintf(dbg_buffer, 2048, "c%d o%d r%d p%d\n%s\nfbs: %d", 
        //     __state.sound_engine._channels, __state.sound_engine._order, __state.sound_engine._row, __state.sound_engine._pattern,
        //     row,
        //     __state.r2dpointer
        // );

        // printf("%s: %p\n", row, strstr(row, "0B"));

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
