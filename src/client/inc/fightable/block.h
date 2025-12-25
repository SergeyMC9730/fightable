
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <fightable/rendered_object.h>
#include <rsb/rsb_array_gen.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 1)

struct flevel_registry_entry;

typedef struct fblock {
    // fblock is suitable for rendering
    struct frendered_object base;

    // light emit level
    unsigned char light_level;

    // constant namыe of the block
    // probably will be implemented differently in the future
    const char *name;

    // block is a starting position for players
    unsigned char is_start_pos : 1;
    // block is unique (only one per level)
    unsigned char singular : 1;
    // block is passable (player does not collide with it)
    unsigned char passable : 1;
    // block can generate different variant of itself
    unsigned char metaobject : 1;
    // block can cause damage
    unsigned char dangerous : 1;
    // reserved for future flags
    unsigned char __reserved__ : 3;

    // registry identifier for this block
    unsigned int registry_id;
    // link to related metaobject
    unsigned short parent_id;
    // layer placement
    unsigned short layer_id;

    // level registry entry associated with this block
    struct flevel_registry_entry *linked_reg;
} fblock;

#pragma pack(pop)

// id + block_x + block_y + bitflags + parent id + layer id + registry id
#define BLOCK_SIZE (sizeof(short) + sizeof(short) + sizeof(short) + sizeof(uint8_t) + sizeof(short) + sizeof(short) + sizeof(int))

// create basic block from block id
struct fblock _fBlockFromId(unsigned short id);

#define INVALID_BLOCK_ID 0xFFFF
// get block id from block structure
unsigned short _fBlockIdFromBlock(struct fblock block);
// try to figure out block id from rendered object
unsigned short _fBlockIdFromRenderable(struct frendered_object obj);

struct fblock_listing {
    // array of available blocks
    const struct fblock *blocks;
    // amount of blocks
    unsigned short total;
};

// get available blocks in the game
struct fblock_listing _fBlockGetAvailable();

// initialize bitfields from compressed 8-bit value
void _fBlockRecoverBitfield(struct fblock *block, unsigned char original);
// compress bitfields into 8-bit value
unsigned char _fBlockGetBitfield(struct fblock* block);

struct flevel;

// update block properties
void _fBlockUpdate(struct fblock* block, struct flevel* env);

RSB_ARRAY_DEF_GEN(struct fblock, _fblock);

#ifdef __cplusplus
}
#endif
