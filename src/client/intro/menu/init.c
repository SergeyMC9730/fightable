
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "fightable/sound_library.h"
#include <fraylib.h>
#include <fightable/intro.h>
#include <fightable/state.h>
#include <fightable/tilemap.h>
#include <fightable/renderer.h>

void _fIntroMenuInit() {
    TraceLog(LOG_INFO, "INITIALIZING MAIN MENU");

    __state.intro_temp_bg = _fTilemapExportTile(__state.tilemap, (IVector2){28.f, 5.f});
    SetTextureWrap(__state.intro_temp_bg, TEXTURE_WRAP_REPEAT);

    int wx = __state.framebuffer.texture.width;
    int wy = __state.framebuffer.texture.height;

    Color text_tint = (Color){0x91, 0xbf, 0xfb, 0xff};
    if (__state.song_id == MUS_CELESTIAL_FANTASIA) {
        text_tint = GREEN;
    }

    Texture2D version_label = _fTextRenderGradientV(&__state.text_manager, "alpha 1.0.0", WHITE, text_tint, 1);
    RenderTexture2D borders_rt = LoadRenderTexture(wx, wy);

    BeginTextureModeStacked(borders_rt);
    ClearBackground(BLANK);

    Color base_tint = BLUE;
    if (__state.song_id == MUS_CELESTIAL_FANTASIA) {
        base_tint = text_tint;
    }

    const int yv0 = ((wy / __state.tilemap->tile_size.y) - 2) * __state.tilemap->tile_size.y;
    const int yv1 = ((wy / __state.tilemap->tile_size.y) - 1) * __state.tilemap->tile_size.y;

    const int xv0 = ((wx / __state.tilemap->tile_size.x) - 1) * __state.tilemap->tile_size.x;

    for (int i = 1; i < (wx / __state.tilemap->tile_size.x) - 1; i++) {
        int x = i * __state.tilemap->tile_size.x;

        _fTilemapDraw(__state.tilemap, (IVector2){x, 0}, (IVector2){24, 5}, 0, 0, base_tint);
        _fTilemapDraw(__state.tilemap, (IVector2){x, yv0}, (IVector2){21, 5}, 0, 0, base_tint);
    }
    for (int i = 1; i < (wy / __state.tilemap->tile_size.y) - 2; i++) {
        int y = i * __state.tilemap->tile_size.y;

        _fTilemapDraw(__state.tilemap, (IVector2){0, y}, (IVector2){20, 5}, 0, 0, base_tint);
        _fTilemapDraw(__state.tilemap, (IVector2){xv0, y}, (IVector2){25, 5}, 0, 0, base_tint);
    }
    _fTilemapDraw(__state.tilemap, (IVector2){0, 0}, (IVector2){23, 5}, 0, 0, base_tint);
    _fTilemapDraw(__state.tilemap, (IVector2){xv0, 0}, (IVector2){27, 5}, 0, 0, base_tint);
    _fTilemapDraw(__state.tilemap, (IVector2){0, yv0}, (IVector2){22, 5}, 0, 0, base_tint);
    _fTilemapDraw(__state.tilemap, (IVector2){xv0, yv0}, (IVector2){26, 5}, 0, 0, base_tint);

    DrawRectangle(0, yv1, wx, yv0, BLACK);

    DrawTexture(version_label, 8, wy - version_label.height - 4, WHITE);

    IVector2 logo_size = (IVector2){11 * __state.tilemap->tile_size.x, 3 * __state.tilemap->tile_size.y};
    int aligned_logo_x = (wx - logo_size.x) / 2;

    logo_size.x /= __state.tilemap->tile_size.x;
    logo_size.y /= __state.tilemap->tile_size.y;

    _fTilemapDrawMegatile(__state.tilemap, (IVector2){aligned_logo_x, 20}, (IVector2){0, 3}, logo_size, 0, 0, WHITE);

    EndTextureModeStacked();

    Image img = LoadImageFromTexture(borders_rt.texture);
    UnloadRenderTexture(borders_rt);

    ImageFlipVertical(&img);

    // ExportImage(img, "test.png");

    __state.menu_borders = LoadTextureFromImage(img);
    UnloadImage(img);
    UnloadTexture(version_label);
}
