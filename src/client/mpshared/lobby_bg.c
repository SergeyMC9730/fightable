
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#ifndef _DISABLE_MP_SERVER_

#include "fightable/renderer.h"
#include <fightable/mp_shared.h>
#include <fightable/state.h>
#include <fightable/sanitizer.h>

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

#endif
