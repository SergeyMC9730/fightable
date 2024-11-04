#include <fightable/block.h>

static struct fblock __blocks[] = {
    {.base.tile_x = 40, .base.tile_y = 7, .name = "Air", .passable = 1},               // 0
    {.base.tile_x = 13, .base.tile_y = 0, .name = "Grass"},             // 1
    {.base.tile_x = 14, .base.tile_y = 0, .name = "Grass"},             // 2
    {.base.tile_x = 14, .base.tile_y = 1, .name = "Grass"},             // 3
    {.base.tile_x = 13, .base.tile_y = 2, .name = "Grass"},             // 4
    {.base.tile_x = 11, .base.tile_y = 2, .name = "Grass"},             // 5
    {.base.tile_x = 17, .base.tile_y = 0, .name = "Grass"},             // 6
    {.base.tile_x = 17, .base.tile_y = 1, .name = "Grass"},             // 7
    {.base.tile_x = 17, .base.tile_y = 2, .name = "Grass"},             // 8
    {.base.tile_x = 11, .base.tile_y = 5, .name = "Grass"},             // 9
    {.base.tile_x = 12, .base.tile_y = 5, .name = "Grass"},             // 10
    {.base.tile_x = 13, .base.tile_y = 5, .name = "Grass"},             // 11
    {.base.tile_x = 16, .base.tile_y = 4, .name = "Grass"},             // 12
    {.base.tile_x = 22, .base.tile_y = 4, .name = "Grass"},             // 13
    {.base.tile_x = 23, .base.tile_y = 4, .name = "Grass"},             // 14
    {.base.tile_x = 24, .base.tile_y = 4, .name = "Grass"},             // 15
    {.base.tile_x = 25, .base.tile_y = 4, .name = "Grass"},             // 16
    {.base.tile_x = 26, .base.tile_y = 4, .name = "Grass"},             // 17
    {.base.tile_x = 27, .base.tile_y = 4, .name = "Grass"},             // 18
    {.base.tile_x = 15, .base.tile_y = 0, .name = "Dirt"},              // 19
    {.base.tile_x = 16, .base.tile_y = 0, .name = "Dirt"},              // 20
    {.base.tile_x = 18, .base.tile_y = 0, .name = "Dirt"},              // 21
    {.base.tile_x = 19, .base.tile_y = 0, .name = "Dirt"},              // 22
    {.base.tile_x = 15, .base.tile_y = 1, .name = "Dirt"},              // 23
    {.base.tile_x = 18, .base.tile_y = 1, .name = "Dirt"},              // 24
    {.base.tile_x = 19, .base.tile_y = 1, .name = "Dirt"},              // 25
    {.base.tile_x = 14, .base.tile_y = 2, .name = "Dirt"},              // 26
    {.base.tile_x = 18, .base.tile_y = 2, .name = "Dirt"},              // 27
    {.base.tile_x = 13, .base.tile_y = 4, .name = "Dirt"},              // 28
    {.base.tile_x = 14, .base.tile_y = 4, .name = "Dirt"},              // 29
    {.base.tile_x = 15, .base.tile_y = 4, .name = "Dirt"},              // 30
    {.base.tile_x = 17, .base.tile_y = 4, .name = "Dirt"},              // 31
    {.base.tile_x = 18, .base.tile_y = 4, .name = "Dirt"},              // 32
    {.base.tile_x = 19, .base.tile_y = 4, .name = "Dirt"},              // 33
    {.base.tile_x = 20, .base.tile_y = 4, .name = "Dirt"},              // 34
    {.base.tile_x = 21, .base.tile_y = 4, .name = "Dirt"},              // 35
    {.base.tile_x = 16, .base.tile_y = 1, .name = "Start", .is_start_pos = 1, .singular = 1, .passable = 1},             // 36
    {.base.tile_x = 28, .base.tile_y = 4, .name = "BG1", .passable = 1},               // 37
    {.base.tile_x = 34, .base.tile_y = 1, .name = "Black"},             // 38
    {.base.tile_x = 11, .base.tile_y = 0, .name = "White"},             // 39
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