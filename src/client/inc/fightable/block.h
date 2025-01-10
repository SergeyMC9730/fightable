#pragma once

#include <fightable/rendered_object.h>
#include <fightable/serializable.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 1)

struct fblock {
    struct frendered_object base;

    unsigned char light_level;

    const char *name;

    unsigned char is_start_pos : 1;
    unsigned char singular : 1;
    unsigned char passable : 1;
    unsigned char metaobject : 1;
    unsigned char dangerous : 1;

    unsigned short group_id;
    unsigned short parent_id;
    unsigned short layer_id;
};

#pragma pack(pop)

// id + block_x + block_y + bitflags
#define BLOCK_SIZE (sizeof(short) + sizeof(short) + sizeof(short) + sizeof(uint8_t))

struct fblock _fBlockFromId(unsigned short id);

#define INVALID_BLOCK_ID 0xFFFF
unsigned short _fBlockIdFromBlock(struct fblock block);
unsigned short _fBlockIdFromRenderable(struct frendered_object obj);

struct fblock_listing {
    const struct fblock *blocks;
    unsigned short total;
};

struct fblock_listing _fBlockGetAvailable();
fserializable _fBlockSerialize(struct fblock block);
struct fblock _fBlockLoad(fserializable *serializable, uint16_t level_version);

struct flevel;

void _fBlockUpdate(struct fblock* block, struct flevel* env);

#ifdef __cplusplus
}
#endif
