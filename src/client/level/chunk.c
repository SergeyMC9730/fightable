
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "fightable/intvec.h"
#include <fightable/block.h>
#include <fightable/level.h>

void _fLevelSplitObjectsIntoChunks(struct flevel *level) {
    if (!level) return;

    if (level->chunks) {
        for (unsigned int i = 0; i < level->chunks->len; i++) {
            _fChunkDestroy(level->chunks->objects + i);
        }
        RSBDestroy_lchunk(level->chunks);
    }

    level->chunks = RSBCreateArray_lchunk();

    IVector2 highest_pos = {};

    for (unsigned int i = 0; i < level->data_size; i++) {
        struct fblock *block = level->objects + i;
        if (block->base.block_x > highest_pos.x) {
            highest_pos.x = block->base.block_x;
        }

    }

    IVector2 cur_offset = {};

    unsigned int added_blocks = 0;
    while (added_blocks < level->data_size) {
        struct flevel_chunk chunk = {};
        chunk.chunk_offset = cur_offset;
    }
}

void _fChunkDestroy(struct flevel_chunk *ch) {
    if (!ch) return;

    if (ch->blocks) {
        RSBDestroy_fblock(ch->blocks);
        ch->blocks = NULL;
    }
    if (IsRenderTextureValid(ch->obj)) {
        UnloadRenderTexture(ch->obj);
        ch->obj = (RenderTexture2D){};
    }
}
