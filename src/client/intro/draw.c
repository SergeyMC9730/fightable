#include <fightable/intro.h>
#include <raylib.h>
#include <fightable/state.h>
#include <stdio.h>
#include <math.h>

void _fIntroDraw() {
    ClearBackground(BLACK);

    double t = _fAudioGetPlayTime(&__state.sound_engine);

    char buffer[20] = {};
    snprintf(buffer, 20, "t: %f\n%d", (float)t, __state.title_song_stage);

    /**
     * Step at 1.080000
     * Step at 1.440000
     * Step at 1.790000
     * Step at 3.960000
    */

    _fTextDraw(&__state.text_manager, buffer, (IVector2){1, 1}, GREEN, 1);

    int w =__state.framebuffer.texture.width;
    int h =__state.framebuffer.texture.height;

    if (t >= 1.08f) {
        __state.title_song_stage = 1;
    }
    if (t >= 1.44f) {
        __state.title_song_stage = 2;
    }
    if (t >= 3.56f) {
        __state.title_song_stage = 3;
    }
    if (t >= 4.f) {
        __state.title_song_stage = 4;
        if (__state.title_r0) {
            __state.title_r0 = 0;
            __state.title_a = 0.f;
        }
    }

    if (IsKeyDown(KEY_SPACE)) {
        printf("Step at %f\n", (float)t);

        __state.title_song_stage++;
    }

    if (__state.title_song_stage >= 1) {
        int size_x = 6 * __state.tilemap->tile_size.x;
        int size_y = 1 * __state.tilemap->tile_size.y;

        int cx = (w - size_x) / 2;
        int cy = (h - size_y) / 2;

        _fTilemapDrawMegatile(*__state.tilemap, (IVector2){cx, cy}, (IVector2){14, 5}, (IVector2){6, 1}, 0, 0, WHITE);
    }

    if (__state.title_song_stage >= 2) {
        int size_x = __state.raylib_logo.width;
        int size_y = 1 * __state.raylib_logo.height;

        int cx = (w - size_x) / 2;
        int cy = h - size_y - 2;

        Color c = WHITE;
        c.r = 16;
        c.g = 16;
        c.b = 16;

        int offset = 6;

        DrawRectangleGradientV(0, cy - offset, w, size_y + 2 + offset, BLACK, c);

        DrawTexture(__state.raylib_logo, cx, cy, WHITE);
    }
    
    float delta = GetFrameTime();

    if (__state.title_song_stage == 3) {
        __state.title_a += delta * 4.5f;

        float v = fmax(1.f - __state.title_a, 0.f);
        Color c = BLACK;
        c.a = 255.f * (1.f - v);

        DrawRectangle(0, 0, w, h, c);

        if (v <= 0.f) {
            __state.title_r0 = 1;
        }
    }
    if (__state.title_song_stage == 4) {
        __state.title_a += delta * 6.f;

        float v = fmax(1.f - __state.title_a, 0.f);
        Color c = BLACK;
        c.a = 255.f * v;

        DrawRectangle(0, 0, w, h, c);
    }

    return;
}