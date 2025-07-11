
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <fightable/intvec.h>

#ifdef __cplusplus
extern "C" {
#endif

struct ftilemap {
    IVector2 tile_size;
    Texture2D texture;
};

struct cJSON;

void _fTilemapDraw(struct ftilemap *tilemap, IVector2 render_pos, IVector2 tile_pos, unsigned char fliped_x, unsigned char fliped_y, Color tint);
void _fTilemapDrawScaled(struct ftilemap *tilemap, IVector2 render_pos, IVector2 tile_pos, unsigned char fliped_x, unsigned char fliped_y, Color tint, float scale);

void _fTilemapDrawMegatile(struct ftilemap *tilemap, IVector2 render_pos, IVector2 start_pos, IVector2 tiles, unsigned char fliped_x, unsigned char fliped_y, Color tint);
void _fTilemapDrawMegatileScaled(struct ftilemap* tilemap, IVector2 render_pos, IVector2 start_pos, IVector2 tiles, unsigned char fliped_x, unsigned char fliped_y, Color tint, float scale);

struct ftilemap _fTilemapCreate(const char *path, IVector2 tile_size);
void _fTilemapUnload(struct ftilemap *tilemap);

Texture2D _fTilemapExportTile(struct ftilemap *tilemap, IVector2 tile_pos);
Image _fTilemapExportTileAsImage(struct ftilemap *tilemap, IVector2 tile_pos);
RLRectangle _fTilemapGetTileRect(struct ftilemap *tilemap, IVector2 tile_pos);

struct cJSON *_fTilemapCreateJson(struct ftilemap *tilemap);
void _fTilemapFromJson(struct ftilemap *tilemap, struct cJSON *j);

#ifdef __cplusplus
}
#endif
