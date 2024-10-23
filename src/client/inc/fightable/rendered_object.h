#pragma once

#pragma pack(push, 1)

#ifdef __cplusplus
extern "C" {
#endif

struct frendered_object {
    unsigned char tile_x;
    unsigned char tile_y;

    unsigned char flipped_x : 1;
    unsigned char flipped_y : 1;

    short block_x;
    short block_y;
};

#ifdef __cplusplus
}
#endif

#pragma pack(pop)