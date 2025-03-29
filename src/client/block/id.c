#include <fightable/block.h>

#define TSET(x,y,n) .base.tile_x = x, .base.tile_y = y, .name = #n

static const struct fblock __blocks[] = {
    {.base.tile_x = 40, .base.tile_y = 7, .name = "Air", .passable = 1},            // 0
    {.base.tile_x = 13, .base.tile_y = 0, .name = "Grass"},                         // 1
    {.base.tile_x = 14, .base.tile_y = 0, .name = "Grass", .metaobject=1},          // 2
    {.base.tile_x = 14, .base.tile_y = 1, .name = "Grass", .parent_id=2},           // 3
    {.base.tile_x = 13, .base.tile_y = 2, .name = "Grass", .parent_id=2},           // 4
    {.base.tile_x = 11, .base.tile_y = 2, .name = "Grass"},                         // 5
    {.base.tile_x = 17, .base.tile_y = 0, .name = "Grass", .metaobject=1},          // 6
    {.base.tile_x = 17, .base.tile_y = 1, .name = "Grass", .parent_id=6},           // 7
    {.base.tile_x = 17, .base.tile_y = 2, .name = "Grass", .parent_id=6},           // 8
    {.base.tile_x = 11, .base.tile_y = 5, .name = "Grass", .metaobject=1},          // 9
    {.base.tile_x = 12, .base.tile_y = 5, .name = "Grass", .parent_id=9},           // 10
    {.base.tile_x = 13, .base.tile_y = 5, .name = "Grass", .parent_id=9},           // 11
    {.base.tile_x = 16, .base.tile_y = 4, .name = "Grass"},                         // 12
    {.base.tile_x = 22, .base.tile_y = 4, .name = "Grass", .metaobject=1},          // 13
    {.base.tile_x = 23, .base.tile_y = 4, .name = "Grass", .parent_id=13},          // 14
    {.base.tile_x = 24, .base.tile_y = 4, .name = "Grass", .parent_id=13},          // 15
    {.base.tile_x = 25, .base.tile_y = 4, .name = "Grass", .metaobject=1},          // 16
    {.base.tile_x = 26, .base.tile_y = 4, .name = "Grass", .parent_id=16},          // 17
    {.base.tile_x = 27, .base.tile_y = 4, .name = "Grass", .parent_id=16},          // 18
    {.base.tile_x = 15, .base.tile_y = 0, .name = "Dirt", .metaobject=1},           // 19
    {.base.tile_x = 16, .base.tile_y = 0, .name = "Dirt", .parent_id=19},           // 20
    {.base.tile_x = 18, .base.tile_y = 0, .name = "Dirt", .metaobject=1},           // 21
    {.base.tile_x = 19, .base.tile_y = 0, .name = "Dirt", .metaobject=1},           // 22
    {.base.tile_x = 15, .base.tile_y = 1, .name = "Dirt", .parent_id=19},           // 23
    {.base.tile_x = 18, .base.tile_y = 1, .name = "Dirt", .parent_id=21},           // 24
    {.base.tile_x = 19, .base.tile_y = 1, .name = "Dirt", .parent_id=22},           // 25
    {.base.tile_x = 14, .base.tile_y = 2, .name = "Dirt", .parent_id=19},           // 26
    {.base.tile_x = 18, .base.tile_y = 2, .name = "Dirt", .parent_id=22},           // 27
    {.base.tile_x = 13, .base.tile_y = 4, .name = "Dirt", .metaobject=1, .passable = 1},        // 28
    {.base.tile_x = 14, .base.tile_y = 4, .name = "Dirt", .parent_id=28, .passable = 1},        // 29
    {.base.tile_x = 15, .base.tile_y = 4, .name = "Dirt", .parent_id=28, .passable = 1},        // 30
    {.base.tile_x = 17, .base.tile_y = 4, .name = "Dirt", .metaobject=1},           // 31
    {.base.tile_x = 18, .base.tile_y = 4, .name = "Dirt", .parent_id=31},           // 32
    {.base.tile_x = 19, .base.tile_y = 4, .name = "Dirt", .parent_id=31},           // 33
    {.base.tile_x = 20, .base.tile_y = 4, .name = "Dirt", .parent_id=31},           // 34
    {.base.tile_x = 21, .base.tile_y = 4, .name = "Dirt", .parent_id=31},           // 35
    {.base.tile_x = 16, .base.tile_y = 1, .name = "Start", .is_start_pos = 1, .passable = 1,},             // 36
    {.base.tile_x = 28, .base.tile_y = 4, .name = "BG1", .passable = 1},            // 37
    {.base.tile_x = 34, .base.tile_y = 1, .name = "Black", .passable=1},            // 38
    {.base.tile_x = 11, .base.tile_y = 0, .name = "White", .passable=1},            // 39
    {TSET(21, 1,Stone), .metaobject = 1},               // 40
    {TSET(21, 0,Stone), .parent_id = 40},               // 41
    {TSET(22, 0,Stone), .parent_id = 40},               // 42
    {TSET(23, 0,Stone), .parent_id = 40},               // 43
    {TSET(24, 0,Stone), .parent_id = 40},               // 44
    {TSET(25, 0,Stone), .parent_id = 40},               // 45
    {TSET(26, 0,Stone), .parent_id = 40},               // 46
    {TSET(22, 1,Magma), .dangerous = 1, .light_level = 24}, // 47
    {TSET(25, 2,BStne), .metaobject = 1},               // 48
    {TSET(19, 2,BStne), .parent_id = 48},               // 49
    {TSET(20, 2,BStne), .parent_id = 48},               // 50
    {TSET(21, 2,BStne), .parent_id = 48},               // 51
    {TSET(22, 2,BStne), .parent_id = 48},               // 52
    {TSET(23, 2,BStne), .parent_id = 48},               // 53
    {TSET(24, 2,BStne), .parent_id = 48},               // 54
    {TSET(20, 1,Planks)},                               // 55
    {TSET(20, 0,Planks)},                               // 56
        {TSET(27, 0,Planks)},                           // 57
    {TSET(28, 2,Aspht)},                                // 58
    {TSET(28, 0,Aspht)},                                // 59
    {TSET(29, 0,Aspht)},                                // 60
    {TSET(30, 0,Aspht)},                                // 61
    {TSET(27, 1,Aspht)},                                // 62
    {TSET(28, 1,Aspht)},                                // 63
    {TSET(29, 1,Aspht)},                                // 64
    {TSET(30, 1,Aspht)},                                // 65
    {TSET(28, 3,Aspht)},                                // 66
    {TSET(30, 3,Bush), .metaobject = 1,.passable=1},            // 67
    {TSET(30, 2,Bush), .parent_id = 67,.passable=1},            // 68
    {TSET(14, 3,Chain)},                                    // 69
    {TSET(29, 3,Lamp), .light_level = 96},                  // 70
    {TSET(29, 2,Lamp), .light_level = 96},                  // 71
    {TSET(11, 3,Grass),.metaobject = 1},                    // 72
    {TSET(12, 3,Grass),.parent_id = 72},                    // 73
    {TSET(13, 3,Grass),.parent_id = 72},                    // 74
    {TSET(15, 3,Grass)},                                    // 75
    {TSET(19, 3,Grass),.metaobject = 1},                    // 76
    {TSET(20, 3,Grass),.parent_id = 76},                    // 77
    {TSET(21, 3,Grass),.parent_id = 76},                    // 78
    {TSET(16, 3,Dirt),.metaobject = 1},                     // 79
    {TSET(17, 3,Dirt),.parent_id = 79},                     // 80
    {TSET(18, 3,Dirt),.parent_id = 79},                     // 81
    {TSET(22, 3,Dirt),.metaobject = 1,.passable=1},         // 82
    {TSET(23, 3,Dirt),.parent_id=82,.passable=1},           // 83
    {TSET(24, 3,Dirt),.parent_id=82,.passable=1},           // 84
    {TSET(12, 4,Grass)},                                    // 85
    {TSET(19, 1,Planks),.metaobject=1},                     // 86
    {TSET(23, 1,Planks)},                                   // 87
    {TSET(24, 1,Planks)},                                   // 88
    {TSET(27, 0,Planks),.parent_id=86},                     // 89
    {TSET(32, 0,Line1),.passable = 1},                      // 90
    {TSET(33, 0,Line1),.passable = 1},                      // 91
    {TSET(32, 1,Line1),.passable = 1},                      // 92
    {TSET(33, 1,Line1),.passable = 1},                      // 93
    {TSET(32, 2,Line1),.passable = 1},                      // 94
    {TSET(33, 2,Line1),.passable = 1},                      // 95
    {TSET(32, 3,Line1),.passable = 1},                      // 96
    {TSET(33, 3,Line1),.passable = 1},                      // 97
    {TSET(20, 5,Line2)},                                    // 98
    {TSET(21, 5,Line2)},                                    // 99
    {TSET(22, 5,Line2)},                                    // 100
    {TSET(23, 5,Line2)},                                    // 101
    {TSET(24, 5,Line2)},                                    // 102
    {TSET(25, 5,Line2)},                                    // 103
    {TSET(26, 5,Line2)},                                    // 104
    {TSET(27, 5,Line2)},                                    // 105
    {TSET(27, 2,Brick)},                                    // 106
    {TSET(34, 0,Wool)},                                     // 107
    {TSET(35, 0,Wool)},                                     // 108
    {TSET(36, 0,Wool)},                                     // 109
    {TSET(37, 0,Wool)},                                     // 110
    {TSET(38, 0,Wool)},                                     // 111
    {TSET(39, 0,Wool)},                                     // 112
    {TSET(40, 0,Wool)},                                     // 113
    {TSET(38, 1,Wool)},                                     // 114
    {TSET(39, 1,Wool)},                                     // 115
    {TSET(40, 1,Wool)},                                     // 116
    {TSET(38, 2,Wool)},                                     // 117
    {TSET(39, 2,Wool)},                                     // 118
    {TSET(40, 2,Wool)},                                     // 119
};

struct fblock _fBlockFromId(unsigned short id) {
    const unsigned short blocks_total = sizeof(__blocks) / sizeof(struct fblock);

    if (id >= blocks_total) {
        struct fblock empty_block = {0};

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
