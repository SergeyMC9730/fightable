#include <fightable/level.h>
#include <fightable/tilemap.h>
#include <fightable/block.h>
#include <stdlib.h>

Rectangle *_fLevelGetHitboxes(struct flevel *level) {
    if (!level) return 0;

    Rectangle *array = (Rectangle *)malloc(level->data_size * sizeof(Rectangle));
    
    for (unsigned int i = 0; i < level->data_size; i++) {
        Rectangle *ref = array + i;
        struct fblock *bref = level->objects + i;

        if (bref->passable) {
            ref->width = 0;
            ref->height = 0;
            ref->x = 0;
            ref->y = 0;

            continue;
        }

        ref->width = level->tilemap->tile_size.x;
        ref->height = level->tilemap->tile_size.y;
        ref->x = bref->base.block_x * ref->width;
        ref->y = bref->base.block_y * ref->height;
    }

    return array;
}