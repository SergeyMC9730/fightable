#include <fightable/intro.h>
#include <raylib.h>
#include <fightable/state.h>
#include <stdio.h>
#include <math.h>
#include <fightable/debug.h>

void _fIntroProcessElectricDrug() {
    double t = _fAudioGetPlayTime(&__state.sound_engine);

    int w =__state.framebuffer.texture.width;
    int h =__state.framebuffer.texture.height;
    float delta = GetFrameTime();

    if (t >= 0.6f) {
        __state.title_song_stage = 1;
    }
    if (t >= 2.15f) {
        __state.title_song_stage = 2;
    }
    if (t >= 4.1f) {
        __state.title_song_stage = 3;
    }
    if (t >= 5.17f) {
        __state.title_song_stage = 4;
    }
    if (t >= 7.75f) {
        __state.title_song_stage = 5;

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

    if (__state.title_song_stage != 5 && !__state.intro_stage_completed) {
        switch (__state.title_song_stage) {
            case 1: {
                int size_x = 6 * __state.tilemap->tile_size.x;
                int size_y = 1 * __state.tilemap->tile_size.y;

                int cx = (w - size_x) / 2;
                int cy = (h - size_y) / 2;

                _fTilemapDrawMegatile(__state.tilemap, (IVector2){cx, cy}, (IVector2){14, 5}, (IVector2){6, 1}, 0, 0, WHITE);

                break;
            }
            case 2: {
                int size_x = __state.raylib_logo.width;
                int size_y = __state.raylib_logo.height;

                int cx = (w - size_x) / 2;
                int cy = (h - size_y) / 2;

                DrawTexture(__state.raylib_logo, cx, cy, WHITE);

                break;
            }
            case 3: {
                int size_x = 11 * __state.tilemap->tile_size.x;
                int size_y = 3 * __state.tilemap->tile_size.y;

                int cx = (w - size_x) / 2;
                int cy = (h - size_y) / 2;

                _fTilemapDrawMegatile(__state.tilemap, (IVector2){cx, cy}, (IVector2){0, 0}, (IVector2){11, 3}, 0, 0, WHITE);

                break;
            }
            case 4: {
                __state.title_a += delta / 2;
                if (__state.title_a > 1.f) {
                    __state.title_a = 1.f;
                }

                Color c = BLACK;
                c.a = 255.f * __state.title_a;

                int size_x = 11 * __state.tilemap->tile_size.x;
                int size_y = 3 * __state.tilemap->tile_size.y;

                int cx = (w - size_x) / 2;
                int cy = (h - size_y) / 2;

                _fTilemapDrawMegatile(__state.tilemap, (IVector2){cx, cy}, (IVector2){0, 0}, (IVector2){11, 3}, 0, 0, WHITE);

                DrawRectangle(0, 0, w, h, c);

                __state.title_r0 = 1;
                
                break;
            }
        }
    } else {
        if (!__state.current_editor) {
            _fIntroMenuDraw();
        }

        __state.intro_stage_completed = 1; 
    }
}