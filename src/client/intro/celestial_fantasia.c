
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "fightable/intvec.h"
#include "fightable/level.h"
#include "fightable/text.h"
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

    if (__state.current_level) {
        __state.current_level->camera.target = (Vector2){__state.cf_level_x, sin(GetTime()) * 8.f - 32};
        __state.current_level->camera_size = (IVector2){w, h};
        __state.cf_level_x += delta * 16.f;
        if ((__state.cf_level_x + (float)w) >= (float)(16 * 16 * __state.tilemap->tile_size.x)) {
            __state.cf_level_x = 0;
        }
        _fLevelDraw(__state.current_level, (IVector2){0, 0});
    }

    double t = _fAudioGetPlayTime(&__state.sound_engine);

    DrawRectangle(__state.gui_render_offset.x, __state.gui_render_offset.y, w, h, (Color){0,0,0,128});

    if (t < 2.f) {
        __state.title_song_stage = 1;
    }
    if (t >= 2.f) {
        __state.title_song_stage = 2;
    }
    if (t >= 5.8f) {
        __state.title_song_stage = 3;
        if (!__state.cf_prepared) {
            _fIntroMenuInit();
            __state.cf_prepared = 1;
        }
    }
    if (t >= 6.9f) {
        __state.title_song_stage = 4;
        if (__state.title_r0) {
            if (!__state.current_editor) {
                // _fIntroMenuInit();
            } else {
                __state.intro_can_continue = 1;
            }

            __state.title_r0 = 0;
            __state.title_a = 0.f;
        }
    }

#ifdef DEBUG
    if (IsKeyDown(KEY_SPACE)) {
        TraceLog(LOG_INFO, "Step at %f\n", (float)t);

        __state.title_song_stage++;
    }
#endif

    if (__state.title_song_stage != 4 && !__state.intro_stage_completed) {
        if (__state.title_song_stage >= 1) {
            {
                int size_x = _fTextMeasure(&__state.text_manager, "dogotrigger").x;
                int size_y = _fTextMeasure(&__state.text_manager, "dogotrigger").y;

                int cx = (w - size_x) / 2;
                int cy = (h - size_y) / 2;

                _fTextDraw(&__state.text_manager, "dogotrigger", (IVector2){cx, cy}, WHITE, 1);
            }
            {
                int size_x = _fTextMeasure(&__state.text_manager, "presents").x;
                int size_y = _fTextMeasure(&__state.text_manager, "presents").y;

                int cx = (w - size_x) / 2;
                int cy = (h - size_y) / 2;

                _fTextDraw(&__state.text_manager, "presents", (IVector2){cx, cy + (__state.text_manager.tilemap.tile_size.y + 5)}, WHITE, 1);
            }
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

            DrawRectangleGradientV(0, cy - offset, w, size_y + 2 + offset, BLANK, c);

            DrawTexture(__state.raylib_logo, cx, cy, WHITE);
        }

        if (__state.title_song_stage == 3) {
            __state.title_a += delta * 1.5f;

            float v = fmax(1.f - __state.title_a, 0.f);
            Color c = WHITE;
            c.a = 255.f * (1.f - v);

            DrawRectangle(0, 0, w, h, c);

            if (v <= 0.f) {
                __state.title_r0 = 1;
            }
    }
    } else {
        __state.title_a += delta * 4.f;

        float v = fmax(1.f - __state.title_a, 0.f);

        if (v <= 1.f) {
            Color c = WHITE;
            c.a = 255.f * v;

            if (!__state.current_editor) {
                if (!__state.cf_prepared_2) {
                    __state.sound_engine.do_not_shake = 0;
                    __state.cf_prepared_2 = 1;
                }
                _fIntroMenuDraw();
            }

            DrawRectangle(0, 0, w, h, c);
        }

        __state.intro_stage_completed = 1;
    }

    float l = 1.f - __state.cf_timer;
    Color c = BLACK;
    c.a = (unsigned char)(255.f * l);
    DrawRectangle(0, 0, w, h, c);

    __state.cf_timer += delta / 4.f;
    if (__state.cf_timer > 1.f) __state.cf_timer = 1.f;
}
