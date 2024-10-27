#include <fightable/intro.h>
#include <raylib.h>
#include <fightable/state.h>
#include <stdio.h>

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

    const int yv0 = ((wy / __state.tilemap->tile_size.y) - 2) * __state.tilemap->tile_size.y;
    const int yv1 = ((wy / __state.tilemap->tile_size.y) - 1) * __state.tilemap->tile_size.y;

    const int xv0 = ((wx / __state.tilemap->tile_size.x) - 1) * __state.tilemap->tile_size.x;

    for (int i = 1; i < (wx / __state.tilemap->tile_size.x) - 1; i++) {
        int x = i * __state.tilemap->tile_size.x;

        _fTilemapDraw(*__state.tilemap, (IVector2){x, 0}, (IVector2){24, 5}, 0, 0, BLUE);
        _fTilemapDraw(*__state.tilemap, (IVector2){x, yv0}, (IVector2){21, 5}, 0, 0, BLUE);
    }
    for (int i = 1; i < (wy / __state.tilemap->tile_size.y) - 2; i++) {
        int y = i * __state.tilemap->tile_size.y;

        _fTilemapDraw(*__state.tilemap, (IVector2){0, y}, (IVector2){20, 5}, 0, 0, BLUE);
        _fTilemapDraw(*__state.tilemap, (IVector2){xv0, y}, (IVector2){25, 5}, 0, 0, BLUE);
    }
    _fTilemapDraw(*__state.tilemap, (IVector2){0, 0}, (IVector2){23, 5}, 0, 0, BLUE);
    _fTilemapDraw(*__state.tilemap, (IVector2){xv0, 0}, (IVector2){27, 5}, 0, 0, BLUE);
    _fTilemapDraw(*__state.tilemap, (IVector2){0, yv0}, (IVector2){22, 5}, 0, 0, BLUE);
    _fTilemapDraw(*__state.tilemap, (IVector2){xv0, yv0}, (IVector2){26, 5}, 0, 0, BLUE);

    DrawRectangle(0, yv1, wx, yv0, BLACK);

    DrawTexture(__state.test_label, 8, wy - __state.test_label.height - 4, WHITE);
}