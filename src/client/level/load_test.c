#include <fightable/level.h>
#include <fightable/camera.h>
#include <fightable/tilemap.h>
#include <fightable/block.h>
#include <stdlib.h>

struct flevel _fLevelLoadTest(struct ftilemap *tilemap, IVector2 background_tile){ 
    struct flevel level = {};
    
    level.width = 32;
    level.height = 8;
    
    level.data_size = level.width * level.height;
    level.objects = (struct fblock *)malloc(sizeof(struct fblock) * level.data_size);
    level.tilemap = tilemap;

    memset(level.objects, 0, sizeof(struct fblock) * level.data_size);

    level.camera = _fCameraLoadDefault();
    level.camera_size = (IVector2){level.width * tilemap->tile_size.x, level.height * tilemap->tile_size.y};

    for (int x = 0; x < level.width; x++) {
        for (int y = 0; y < level.height; y++) {
            int idx = (y * level.width) + x;

            struct fblock *obj = level.objects + idx;

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

    level.background_tile = _fTilemapExportTile(tilemap, background_tile);
    SetTextureWrap(level.background_tile, TEXTURE_WRAP_REPEAT);

    return level;
}