
//          Sergei Baigerov 2024 - 2026.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "fightable/renderer.h"
#include <fightable/mp_shared.h>
#include <fightable/state.h>
#include <fightable/sanitizer.h>
#include <fightable/storage.h>

void _fMpDrawLobbyBackground(float opacity) {
    opacity = _fSanitizeFloat(opacity, 0.f, 1.f);

    BeginTextureModeStacked(__state.mp_lobby_bg);

    Color bg = {
		243, 134, 5, 255
	};

	RLRectangle dest = {
		.width = (float)__state.mp_create_bg1.width,
		.height = (float)__state.mp_create_bg1.height
	};

	RLRectangle r1 = {
		.x = -(float)__state.mp_create_time * 10.f,
		.y = 0,
		.width = dest.width,
		.height = dest.height,
	};
	RLRectangle r2 = r1;
	r2.x *= 0.7f;

	DrawRectangle(0, 0, __state.framebuffer.texture.width, __state.framebuffer.texture.height, bg);

	int w = __state.framebuffer.texture.width / 3;
	Color rbcol = WHITE;
	rbcol.a = 64;

	Color wavecol = WHITE;
	wavecol.a = 32;

	BeginBlendMode(BLEND_ADDITIVE);

	DrawTexturePro(__state.mp_create_bg1, r1, dest, (Vector2) {}, 0.f, WHITE);
	DrawTexturePro(__state.mp_create_bg2, r2, dest, (Vector2) {}, 0.f, WHITE);

	if (IsShaderValid(__state.mp_create_wave_shader)) BeginShaderMode(__state.mp_create_wave_shader);

	r1 = (RLRectangle){
		.x = -(float)__state.mp_create_time * 15.f,
		.y = 0,
		.width = dest.width,
		.height = dest.height,
	};
	r2 = r1;
	r2.x *= 0.5f;

	DrawTexturePro(__state.mp_create_bg1, r1, dest, (Vector2) {}, 0.f, wavecol);
	DrawTexturePro(__state.mp_create_bg2, r2, dest, (Vector2) {}, 0.f, wavecol);

	if (IsShaderValid(__state.mp_create_wave_shader)) EndShaderMode();
	DrawRectangleGradientH(0, 0, w, __state.framebuffer.texture.height, rbcol, BLACK);
	EndBlendMode();

	w = __state.framebuffer.texture.width / 2;
	int h = __state.framebuffer.texture.height / 2;

	BeginBlendMode(BLEND_MULTIPLIED);
	DrawRectangleGradientH(__state.framebuffer.texture.width - w, 0, w, __state.framebuffer.texture.height, WHITE, BLACK);
	DrawRectangleGradientV(0, __state.framebuffer.texture.height - h, w * 2, h, WHITE, BLACK);
	EndBlendMode();

	__state.mp_create_time += (long double)GetFrameTime();

	EndTextureModeStacked();

	Color bg_col = WHITE;
	bg_col.a = (unsigned char)(255.f * opacity);
	_fDrawRenderTexture(__state.mp_lobby_bg, (Vector2){0, 0}, bg_col, 1.f, (Vector2){0, 0}, (Vector2){0, 0});
}

void _fMpCreateLobbyBackground() {
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

    char* buffer = (char*)MemAlloc(256);
    const char* readable = _fStorageGetWritable();

#ifndef GRAPHICS_API_OPENGL_ES3
    snprintf(buffer, 256, "%s/assets/shaders/wave_warp.fs", readable);
#else
    snprintf(buffer, 256, "%s/assets/shaders/wave_warp_es3.fs", readable);
#endif
    __state.mp_create_wave_shader = LoadShader(NULL, buffer);

    MemFree(buffer);

    __state.mp_lobby_bg = LoadRenderTexture(__state.framebuffer.texture.width, __state.framebuffer.texture.height);
    __state.mp_lobby_bg_ready = 1;
}
