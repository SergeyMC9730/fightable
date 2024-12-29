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
#include <fightable/storage.h>
#include <fightable/sound_library.h>
#include <fightable/mp_create_menu.h>
#include <math.h>

void _fIntroMenuOnMpCreateCallback(void *ctx) {
    float* old_vol = (float*)ctx;
    if (!old_vol) return;

    char* buffer = (char*)MemAlloc(256);
    const char* readable = _fStorageGetWritable();

    _fAudioStop(&__state.sound_engine);

    Image pattern_image1 = GenImageColor(__state.framebuffer.texture.width, __state.framebuffer.texture.height, BLANK);
    Image pattern_image2 = ImageCopy(pattern_image1);

    static const double gap = 5;
    for (unsigned int x = 0; x < pattern_image1.width; x++) {
        double _x = (double)x / ((double)pattern_image1.width / (double)PI);
        double _x2 = ((double)x + ((double)pattern_image1.width / 2)) / ((double)pattern_image1.width / (double)PI);

        double v1 = fabs(sin(_x) * (((double)pattern_image1.height / 2) - gap));
        double v2 = fabs(sin(_x) * (((double)pattern_image1.height / 2) - gap)) * -1 + (double)pattern_image1.height;
        double v3 = fabs(sin(_x2) * (((double)pattern_image1.height / 2) - (gap + ((double)pattern_image1.height / 3)))) + gap;
        double v4 = fabs(sin(_x2) * (((double)pattern_image1.height / 2) - (gap + ((double)pattern_image1.height / 3)))) * -1 + (double)pattern_image1.height - gap;
    
        Color col = WHITE;
        col.a = 24;

        for (unsigned int y = 0; y <= (int)v1; y++) {
            ImageDrawPixel(&pattern_image1, x, y, col);
        }
        for (unsigned int y = 0; y <= (int)v3; y++) {
            ImageDrawPixel(&pattern_image2, x, y, col);
        }

        for (unsigned int y = pattern_image1.height; y >= (int)v2; y--) {
            ImageDrawPixel(&pattern_image1, x, y, col);
        }
        for (unsigned int y = pattern_image1.height; y >= (int)v4; y--) {
            ImageDrawPixel(&pattern_image2, x, y, col);
        }

        // printf("x=%d, r: %f, %f, %f, %f\n", x, (float)v1, (float)v2, (float)v3, (float)v4);
    }

    __state.mp_create_bg1 = LoadTextureFromImage(pattern_image1);
    __state.mp_create_bg2 = LoadTextureFromImage(pattern_image2);
    UnloadImage(pattern_image1);
    UnloadImage(pattern_image2);

    SetTextureWrap(__state.mp_create_bg1, TEXTURE_WRAP_REPEAT);
    SetTextureWrap(__state.mp_create_bg2, TEXTURE_WRAP_REPEAT);

    snprintf(buffer, 256, "%s/wave_warp.fs", readable);
    __state.mp_create_wave_shader = LoadShader(NULL, buffer);

    snprintf(buffer, 256, "%s/3g_crim.xm", readable);
    _fAudioPlayModule(&__state.sound_engine, buffer);
    _fAudioLoopCurrent(&__state.sound_engine);
    _fAudioFxSlideVolume(&__state.sound_engine, *old_vol, 0.5f);

    MemFree(old_vol);
    MemFree(buffer);

    __state.song_id = MUS_3G_ANTS;
    __state.current_ui_menu = UI_MENU_MPCREATE;
    __state.mp_create_time = 0;

    _fMpCreateOpenServer();
}

void _fIntroMenuOnMpCreate() {
    float* old_vol = (float *)MemAlloc(sizeof(float));
    *old_vol = _fAudioGetVolume(&__state.sound_engine);

    _fGfxFadeOut(&__state.gfx, BLACK, 0.5);
    _fGfxSetFadeOutFunc(&__state.gfx, _fIntroMenuOnMpCreateCallback, old_vol);
    
    _fAudioFxSlideVolume(&__state.sound_engine, 0.f, 0.4f);
}