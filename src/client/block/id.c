#include <fightable/block.h>

static struct fblock __blocks[] = {
    {.base.tile_x = 40, .base.tile_y = 7, .name = "Air", .passable = 1},    // 0
    {.base.tile_x = 13, .base.tile_y = 0, .name = "Grass"},                 // 1
    {.base.tile_x = 14, .base.tile_y = 0, .name = "Grass", .metaobject=1},  // 2
    {.base.tile_x = 14, .base.tile_y = 1, .name = "Grass", .parent_id=2},   // 3
    {.base.tile_x = 13, .base.tile_y = 2, .name = "Grass", .parent_id=2},   // 4
    {.base.tile_x = 11, .base.tile_y = 2, .name = "Grass"},                 // 5
    {.base.tile_x = 17, .base.tile_y = 0, .name = "Grass", .metaobject=1},  // 6
    {.base.tile_x = 17, .base.tile_y = 1, .name = "Grass", .parent_id=6},   // 7
    {.base.tile_x = 17, .base.tile_y = 2, .name = "Grass", .parent_id=6},   // 8
    {.base.tile_x = 11, .base.tile_y = 5, .name = "Grass", .metaobject=1},  // 9
    {.base.tile_x = 12, .base.tile_y = 5, .name = "Grass", .parent_id=9},   // 10
    {.base.tile_x = 13, .base.tile_y = 5, .name = "Grass", .parent_id=9},   // 11
    {.base.tile_x = 16, .base.tile_y = 4, .name = "Grass"},                 // 12
    {.base.tile_x = 22, .base.tile_y = 4, .name = "Grass", .metaobject=1},  // 13
    {.base.tile_x = 23, .base.tile_y = 4, .name = "Grass", .parent_id=13},  // 14
    {.base.tile_x = 24, .base.tile_y = 4, .name = "Grass", .parent_id=13},  // 15
    {.base.tile_x = 25, .base.tile_y = 4, .name = "Grass", .metaobject=1},  // 16
    {.base.tile_x = 26, .base.tile_y = 4, .name = "Grass", .parent_id=16},  // 17
    {.base.tile_x = 27, .base.tile_y = 4, .name = "Grass", .parent_id=16},  // 18
    {.base.tile_x = 15, .base.tile_y = 0, .name = "Dirt", .metaobject=1},   // 19
    {.base.tile_x = 16, .base.tile_y = 0, .name = "Dirt", .parent_id=19},   // 20
    {.base.tile_x = 18, .base.tile_y = 0, .name = "Dirt", .metaobject=1},   // 21
    {.base.tile_x = 19, .base.tile_y = 0, .name = "Dirt", .metaobject=1},   // 22
    {.base.tile_x = 15, .base.tile_y = 1, .name = "Dirt", .parent_id=19},   // 23
    {.base.tile_x = 18, .base.tile_y = 1, .name = "Dirt", .parent_id=21},   // 24
    {.base.tile_x = 19, .base.tile_y = 1, .name = "Dirt", .parent_id=22},   // 25
    {.base.tile_x = 14, .base.tile_y = 2, .name = "Dirt", .parent_id=19},   // 26
    {.base.tile_x = 18, .base.tile_y = 2, .name = "Dirt", .parent_id=22},   // 27
    {.base.tile_x = 13, .base.tile_y = 4, .name = "Dirt", .metaobject=1},   // 28
    {.base.tile_x = 14, .base.tile_y = 4, .name = "Dirt", .parent_id=28},   // 29
    {.base.tile_x = 15, .base.tile_y = 4, .name = "Dirt", .parent_id=28},   // 30
    {.base.tile_x = 17, .base.tile_y = 4, .name = "Dirt", .metaobject=1},   // 31
    {.base.tile_x = 18, .base.tile_y = 4, .name = "Dirt", .parent_id=31},   // 32
    {.base.tile_x = 19, .base.tile_y = 4, .name = "Dirt", .parent_id=31},   // 33
    {.base.tile_x = 20, .base.tile_y = 4, .name = "Dirt", .parent_id=31},   // 34
    {.base.tile_x = 21, .base.tile_y = 4, .name = "Dirt", .parent_id=31},   // 35
    {.base.tile_x = 16, .base.tile_y = 1, .name = "Start", .is_start_pos = 1, .singular = 1, .passable = 1},             // 36
    {.base.tile_x = 28, .base.tile_y = 4, .name = "BG1", .passable = 1},    // 37
    {.base.tile_x = 34, .base.tile_y = 1, .name = "Black"},                 // 38
    {.base.tile_x = 11, .base.tile_y = 0, .name = "White"},                 // 39
};

struct fblock _fBlockFromId(unsigned short id) {
    const unsigned short blocks_total = sizeof(__blocks) / sizeof(struct fblock);
    
    if (id >= blocks_total) {
        struct fblock empty_block = {};

        return empty_block;
    }

    return __blocks[id];
}

unsigned short _fBlockIdFromBlock(struct fblock block) {
    const unsigned short blocks_total = sizeof(__blocks) / sizeof(struct fblock);

    for (unsigned short i = 0; i < blocks_total; i++) {
        if (__blocks[i].base.tile_x == block.base.tile_x && __blocks[i].base.tile_y == block.base.tile_y) {
            return i;
        }
    }

    return INVALID_BLOCK_ID;
}
unsigned short _fBlockIdFromRenderable(struct frendered_object obj) {
    const unsigned short blocks_total = sizeof(__blocks) / sizeof(struct fblock);

    for (unsigned short i = 0; i < blocks_total; i++) {
        if (__blocks[i].base.tile_x == obj.tile_x && __blocks[i].base.tile_y == obj.tile_y) {
            return i;
        }
    }

    return INVALID_BLOCK_ID;
}

struct fblock_listing _fBlockGetAvailable() {
    struct fblock_listing l = {0};
    l.blocks = __blocks;
    l.total = sizeof(__blocks) / sizeof(struct fblock);

    return l; 
}