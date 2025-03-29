
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "fightable/intvec.h"
#include "fightable/level.h"
#include "raylib.h"
#include <fightable/intro.h>
#include <fraylib.h>
#include <fightable/state.h>
#include <math.h>
#include <fightable/debug.h>

void _fIntroProcessCelestialFantasia() {
    float delta = GetFrameTime();
    int w =__state.framebuffer.texture.width;
    int h =__state.framebuffer.texture.height;

    __state.current_level->camera.target = (Vector2){__state.cf_level_x, sin(GetTime()) * 8.f - 64 + (__state.cf_level_x / 6.f)};
    __state.current_level->camera_size = (IVector2){w, h};
    __state.cf_level_x += delta * 3.f;
    _fLevelDraw(__state.current_level, (IVector2){0, 0});

    double t = _fAudioGetPlayTime(&__state.sound_engine);

    DrawRectangle(__state.gui_render_offset.x, __state.gui_render_offset.y, w, h, (Color){0,0,0,128});

    if (t >= 1.08f) {
        __state.title_song_stage = 1;
    }
    if (t >= 1.8f) {
        __state.title_song_stage = 2;
    }
    if (t >= 4.06f) {
        __state.title_song_stage = 3;
    }
    if (t >= 4.45f) {
        __state.title_song_stage = 4;
        if (__state.title_r0) {
            if (!__state.current_editor) {
                _fIntroMenuInit();
            } else {
                __state.intro_can_continue = 1;
            }

            __state.title_r0 = 0;
            __state.title_a = 0.f;
        }
    }

#ifdef DEBUG
    if (IsKeyDown(KEY_SPACE)) {
        printf("Step at %f\n", (float)t);

        __state.title_song_stage++;
    }
#endif

    if (__state.title_song_stage != 4 && !__state.intro_stage_completed) {
        if (__state.title_song_stage >= 1) {
            int size_x = 6 * __state.tilemap->tile_size.x;
            int size_y = 1 * __state.tilemap->tile_size.y;

            int cx = (w - size_x) / 2;
            int cy = (h - size_y) / 2;

            _fTilemapDrawMegatile(__state.tilemap, (IVector2){cx, cy}, (IVector2){14, 5}, (IVector2){6, 1}, 0, 0, WHITE);
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
    } else {
        __state.title_a += delta * 6.f;

        float v = fmax(1.f - __state.title_a, 0.f);

        if (v <= 1.f) {
            Color c = BLACK;
            c.a = 255.f * v;

            if (!__state.current_editor) {
                _fIntroMenuDraw();
            }

            DrawRectangle(0, 0, w, h, c);
        }

        __state.intro_stage_completed = 1;
    }
}
