#include <fightable/level.h>
#include <fightable/camera.h>
#include <fightable/tilemap.h>
#include <fightable/block.h>
#include <stdlib.h>

struct flevel _fLevelLoadTest(struct ftilemap *tilemap, IVector2 background_tile){ 
    struct flevel level = {};
    
    level.width = 8;
    level.height = 1;
    
    level.data_size = level.width * level.height;
    level.objects = (struct fblock *)malloc(sizeof(struct fblock) * level.data_size);
    level.tilemap = tilemap;

    level.camera = _fCameraLoadDefault();
    level.camera_size = (IVector2){level.width * tilemap->tile_size.x, level.height * tilemap->tile_size.y};

    for (int x = 0; x < level.width; x++) {
        for (int y = 0; y < level.height; y++) {
            int idx = (y * level.width) + x;

            struct fblock *obj = level.objects + idx;

            obj->base.tile_x = 11;
            obj->base.tile_y = 0;

            obj->base.block_x = x;
            obj->base.block_y = y + 3;
        }
    }

    level.background_tile = _fTilemapExportTile(tilemap, background_tile);
    SetTextureWrap(level.background_tile, TEXTURE_WRAP_REPEAT);

    return level;
}