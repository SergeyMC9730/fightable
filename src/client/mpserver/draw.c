#include <fightable/mp_create_menu.h>
#include <fightable/state.h>
#include <stdio.h>

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

	BeginShaderMode(__state.mp_create_wave_shader);

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

	EndShaderMode();
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

	_fTextDraw(&__state.text_manager, buffer, (IVector2) { 8, 8 }, BLACK, 1);

	_fTextDraw(&__state.text_manager, "join now!", (IVector2) { 8, 24 }, BLACK, 1);

	int tx = __state.tilemap->tile_size.x;
	int ty = __state.tilemap->tile_size.y;
	
	int offset = ((tx * 2) + 6);

	for (int i = 0; i < _fTcpSrvGetConnectedUsers(__state.mp_server_instance); i++) {
		_fTilemapDrawScaled(__state.tilemap, (IVector2) { 16 + (offset * i) + 2, 80 + 2 }, (IVector2) { 12, 0 }, 0, 0, BLACK, 2.f);
		_fTilemapDrawScaled(__state.tilemap, (IVector2) { 16 + (offset * i), 80 }, (IVector2) { 12, 0 }, 0, 0, WHITE, 2.f);

		const char* nickname = "Player";
		IVector2 len = _fTextMeasure(&__state.text_manager, nickname);

		int center = ((tx * 2) - len.x) / 2;

		_fTextDraw(&__state.text_manager, nickname, (IVector2) { 16 + (offset * i) + center, ((i % 2) == 0) ? 100 : (100 - (tx * 3.5)) }, WHITE, 1);
	}
#endif
}