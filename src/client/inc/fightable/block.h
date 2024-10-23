#pragma once

#include <fightable/rendered_object.h>

#ifdef __cplusplus
extern "C" {
#endif

struct fblock {
    struct frendered_object base;
    const char *name;

    unsigned char is_start_pos : 1;
    unsigned char singular : 1;
    unsigned char passable : 1;
};

struct fblock _fBlockFromId(unsigned short id);

#define INVALID_BLOCK_ID 0xFFFF
unsigned short _fBlockIdFromBlock(struct fblock block);
unsigned short _fBlockIdFromRenderable(struct frendered_object obj);

struct fblock_listing {
    struct fblock *blocks;
    unsigned short total;
};

struct fblock_listing _fBlockGetAvailable();

#ifdef __cplusplus
}
#endif