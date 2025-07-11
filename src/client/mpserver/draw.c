#include "fightable/storage.h"
#include <fightable/mp_create_menu.h>
#include <fightable/state.h>
#include <stdio.h>
#include <fightable/rect.h>
#include <fightable/level.h>
#include <fightable/button.h>
#include <fightable/renderer.h>
#include <fightable/camera.h>
#ifndef TARGET_ANDROID
#include <nfd.h>
#endif

void _fMpOnOpenLevel(struct nt_file_selector_menu *ctx, const char *path) {
    struct flevel *lvl = _fLevelLoadFromFileSelector(path);

    if (lvl != __state.current_level && __state.current_level) {
        unsigned char src = __state.current_level->level_source;
        _fLevelDestroy(__state.current_level, 1, (src != LEVEL_SOURCE_EDITOR), (src != LEVEL_SOURCE_EDITOR));
        __state.current_level = NULL;
    }

    __state.current_level = lvl;

    _fCloseFileSelector();
}

void _fMpCreateDraw() {
#ifndef _DISABLE_MP_SERVER_
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

	if (!_fTcpSrvReady(__state.mp_server_instance)) return;

	char buffer[32] = {};
	snprintf(buffer, 32, "running on port\n %d", __state.mp_server_port);

	_fTextDraw(&__state.text_manager, buffer, (IVector2) { 8, 8 }, BLACK, 0);

	_fTextDraw(&__state.text_manager, "join now!", (IVector2) { 8, 24 }, BLACK, 0);

	int tx = __state.tilemap->tile_size.x;
	int ty = __state.tilemap->tile_size.y;

	int offset = ((tx * 2) + 6);

	for (int i = 0; i < 6; i++) {
		_fTilemapDrawScaled(__state.tilemap, (IVector2) { 16 + (offset * i) + 2, 80 + 2 }, (IVector2) { 12, 0 }, 0, 0, BLACK, 2.f);
		_fTilemapDrawScaled(__state.tilemap, (IVector2) { 16 + (offset * i), 80 }, (IVector2) { 12, 0 }, 0, 0, WHITE, 2.f);

		const char* nickname = "Player";
		IVector2 len = _fTextMeasure(&__state.text_manager, nickname);

		int center = ((tx * 2) - len.x) / 2;

		_fTextDraw(&__state.text_manager, nickname, (IVector2) { 16 + (offset * i) + center, ((i % 2) == 0) ? 100 : (100 - (tx * 3.5)) }, WHITE, 1);
	}

	IVector2 sel_sz = _fTextMeasure(&__state.text_manager, "select level");
	_fTextDraw(&__state.text_manager, "select level", (IVector2) { 96, 8 }, YELLOW, 1);

	RLRectangle area = { 96, 16, sel_sz.x, 16 };

	Color container_color = BLACK;
	container_color.a = 64;
	_fRectDraw(area, YELLOW, ORANGE, container_color);

	if (__state.current_level != NULL) {
		_fLevelDrawPixelated(__state.current_level, (IRectangle) { area.x + 1, area.y + 1, area.width - 1, area.height - 1 });

		Vector2 mouse = _fGetMousePosPix();

		if (CheckCollisionPointRec(mouse, area) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			Vector2 d = GetMouseDelta();

			__state.current_level->camera.target.x -= d.x / ((float)__state.current_level->tilemap->tile_size.x / 1.5f);
			__state.current_level->camera.target.y -= d.y / ((float)__state.current_level->tilemap->tile_size.y / 1.5f);
		}
	}

	if (_fButtonDrawSimple("Open", (IVector2) { 108, 17 + 15 + 2 }, WHITE)) {
	    _fOpenFileSelector(_fStorageGetWritable(), _fMpOnOpenLevel);
	}
#endif
}
