#include <fightable/state.h>
#include <fightable/tilemap.h>
#include <fightable/renderer.h>
#include <fightable/level.h>
#include <stdlib.h>
#include <string.h>
#include <fightable/editor.h>
#include <stdio.h>

struct flevel __level;
struct ftilemap __tilemap;

int main(int argc, char **argv) {
    Vector2 win_sz = {800, 600};
    Vector2 editor_sz = {255, 0};

    InitWindow(win_sz.x, win_sz.y, "Fightable");
    SetTargetFPS(60);

    __tilemap = _fTilemapCreate("assets/fightable1.png", (IVector2){8, 8});
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

    RenderTexture2D txt = LoadRenderTexture(win_sz.x / 5, win_sz.y / 5);
    __state.framebuffer = txt;

    while (!WindowShouldClose()) {
        BeginDrawing();
        BeginTextureMode(__state.framebuffer);

        _fDraw();

        EndTextureMode();

        Rectangle source = (Rectangle){ 0, 0, (float)__state.framebuffer.texture.width, (float)-__state.framebuffer.texture.height };
        Rectangle dest = (Rectangle){ 0, 0, GetRenderWidth(), GetRenderHeight() };

        DrawTexturePro(__state.framebuffer.texture, source, dest, (Vector2){0, 0}, 0.f, WHITE);

        // DrawFPS(4, 4);

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

    return 0;
}
