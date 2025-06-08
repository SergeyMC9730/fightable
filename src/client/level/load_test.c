
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/level.h>
#include <fightable/camera.h>
#include <fightable/tilemap.h>
#include <fightable/block.h>
#include <fightable/pthread_compat.h>
#include <pthread.h>
#include <stdlib.h>

struct flevel *_fLevelLoadTest(struct ftilemap *tilemap, IVector2 background_tile){
    struct flevel *level = (struct flevel *)malloc(sizeof(struct flevel));
    memset(level, 0, sizeof(struct flevel));

    level->width = 32;
    level->height = 8;

    level->data_size = level->width * level->height;
    level->objects = (struct fblock *)malloc(sizeof(struct fblock) * level->data_size);
    level->tilemap = tilemap;

    memset(level->objects, 0, sizeof(struct fblock) * level->data_size);

    level->camera = _fCameraLoadDefault();
    level->camera_size = (IVector2){level->width * tilemap->tile_size.x, level->height * tilemap->tile_size.y};

    level->block_entries = RSBCreateArray_lre();

    for (int x = 0; x < level->width; x++) {
        for (int y = 0; y < level->height; y++) {
            int idx = (y * level->width) + x;

            struct fblock *obj = level->objects + idx;

            if (y == 0) {
                *obj = _fBlockFromId(2);
            }
            else {
                *obj = _fBlockFromId(19);
            }

            obj->base.block_x = x;
            obj->base.block_y = y + 3;
        }
    }

    level->background_tile = _fTilemapExportTile(tilemap, background_tile);
    SetTextureWrap(level->background_tile, TEXTURE_WRAP_REPEAT);

    level->tps = 50.f;
    level->block_processor_thread = (PTHREAD_TYPE)0;

    level->level_source = LEVEL_SOURCE_TEMPLATE;

    _fLevelLoadProcessor(level);

    return level;
}
