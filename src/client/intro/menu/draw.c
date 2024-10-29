#include <fightable/intro.h>
#include <raylib.h>
#include <fightable/state.h>
#include <stdio.h>
#include <fightable/button.h>
#include <stdlib.h>
#include <fightable/rect.h>
#include <fightable/renderer.h>
#include <fightable/level.h>
#include <fightable/editor.h>

void _fIntroMenuDraw() {
    float delta = GetFrameTime();
    float speed = 24.f;
    __state.intro_bg_offsetf += delta * speed;

    int wx = __state.framebuffer.texture.width;
    int wy = __state.framebuffer.texture.height;

    Rectangle source = {};
    source.width = wx;
    source.height = wy;

    source.x -= ((long)__state.intro_bg_offsetf % __state.intro_temp_bg.width);
    source.y += ((long)__state.intro_bg_offsetf % __state.intro_temp_bg.height);

    Rectangle dest = source;
    dest.x = 0;
    dest.y = 0;

    Color bg_color = (Color){0, 0, 200, 255};

    DrawTexturePro(__state.intro_temp_bg, source, dest, (Vector2){}, 0.f, bg_color);

    Color begin = BLACK;
    begin.a = 200;
    Color end = WHITE;
    end.a = 128;

    BeginBlendMode(BLEND_MULTIPLIED);

    DrawRectangleGradientV(0, 0, wx, wy, begin, end);

    EndBlendMode();

    // ClearBackground(BLUE);

    DrawTexture(__state.menu_borders, 0, 0, WHITE);
    if (__state.menu_pressed_play) {
        int w = 80;
        Rectangle area = (Rectangle){(wx - w) / 2, 45, w, 50};
        if (_fButtonDrawSimple("BACK", (IVector2){(wx - (3 * __state.tilemap->tile_size.x)) / 2, area.y + area.height - 1})) {
            __state.menu_pressed_play = 0;
            UnloadTexture(__state.playbtn_container);
        } else {
            area.width = __state.playbtn_container.width;
            area.height = __state.playbtn_container.height;

            // DrawRectangleRec(area, (Color){0, 0, 0, 64});
            DrawTexture(__state.playbtn_container, area.x, area.y, WHITE);   
        }
    } else {
        if (_fButtonDrawSimple("PLAY", (IVector2){(wx - (3 * __state.tilemap->tile_size.x)) / 2, 50})) {
            __state.menu_pressed_play = 1;
            Texture2D singleplayer_label = _fTextRenderGradientV(&__state.text_manager, "Singleplayer", WHITE, (Color){0x91, 0xbf, 0xfb, 0xff}, 1);
            Texture2D multiplayer_label = _fTextRenderGradientV(&__state.text_manager, "Multiplayer", WHITE, (Color){0x91, 0xbf, 0xfb, 0xff}, 1);

            int w = 80;
            Rectangle area = (Rectangle){(wx - w) / 2, 45, w, 50};

            RenderTexture2D rt2d = LoadRenderTexture(area.width, area.height);
            BeginTextureModeStacked(rt2d);
            ClearBackground((Color){0, 0, 0, 64}); // (Color){0, 0, 0, 160}

            _fRectDraw((Rectangle){0, 0, area.width - 1, area.height - 1}, WHITE, (Color){0x71, 0xaf, 0xfb, 0xff}, (Color){0, 0, 0, 0});

            int offset = 3;

            BeginScissorMode(1, 1, area.width - 1, area.height - 1);

            // DrawRectanglePro((Rectangle){-10 + area.x, 15 + area.y, 15, 15}, (Vector2){}, 45.f, GREEN);
            DrawRectangle(0, offset, 15, 15, RED);
            DrawRectanglePro((Rectangle){3, 0 + offset, 15, 15}, (Vector2){}, 45.f, (Color){0, 0, 0, 16});
            DrawRectanglePro((Rectangle){1, 0 + offset, 15, 15}, (Vector2){}, 45.f, (Color){0, 0, 0, 16});
            DrawRectanglePro((Rectangle){0, 0 + offset, 15, 15}, (Vector2){}, 45.f, GREEN);
            DrawTexture(multiplayer_label, 27, 6 + offset, WHITE);

            offset -= 2;

            DrawRectanglePro((Rectangle){8, 26 + offset, 15, 15}, (Vector2){}, 65.f, RED);
            DrawTexture(singleplayer_label, 25, 33 + offset, WHITE);

            DrawLine(2, 23 + offset, area.width - 2, 23 + offset, (Color){0x71, 0xaf, 0xfb, 0xff});

            EndScissorMode();

            EndTextureModeStacked();

            UnloadTexture(multiplayer_label);
            UnloadTexture(singleplayer_label);

            Image img = LoadImageFromTexture(rt2d.texture);
            ImageFlipVertical(&img);

            __state.playbtn_container = LoadTextureFromImage(img);
        
            UnloadImage(img);
            UnloadRenderTexture(rt2d);
        }
        
        if (_fButtonDrawSimple("OPTIONS", (IVector2){(wx - (4 * __state.tilemap->tile_size.x)) / 2, 60})) {

        }
        if (_fButtonDrawSimple("EDITOR", (IVector2){(wx - (4 * __state.tilemap->tile_size.x)) / 2, 70})) {
            __state.current_editor = _fEditorCreate();
            free(__state.current_level->objects);
            __state.current_level->objects = NULL;

            __state.current_level = NULL;

            __state.initial_game_size.x += __state.editor_size.x;
            __state.initial_game_size.y += __state.editor_size.y;

            __state.intro_can_continue = 1;

#ifndef TARGET_ANDROID
            SetWindowSize(__state.initial_game_size.x, __state.initial_game_size.y);
#endif
            SetWindowTitle("Fightable Editor");

            UnloadRenderTexture(__state.framebuffer);
            __state.framebuffer = LoadRenderTexture(__state.initial_game_size.x / UI_SCALE, __state.initial_game_size.y / UI_SCALE);
        }
        if (_fButtonDrawSimple("EXIT", (IVector2){(wx - (3 * __state.tilemap->tile_size.x)) / 2, 80})) {
            exit(0);
        }
    }
}