#include <fightable/block.h>

void _fBlockRecoverBitfield(struct fblock* block, unsigned char original) {
    block->is_start_pos = (original >> 0) & 0x01;
    block->singular     = (original >> 1) & 0x01;
    block->passable     = (original >> 2) & 0x01;
    block->metaobject   = (original >> 3) & 0x01;
    block->dangerous    = (original >> 4) & 0x01;
    block->__reserved__ = (original >> 5) & 0x07;
}
unsigned char _fBlockGetBitfield(struct fblock* block) {
    unsigned char b = 0;

    b |= block->is_start_pos << 0;
    b |= block->singular << 1;
    b |= block->passable << 2;
    b |= block->metaobject << 3;
    b |= block->dangerous << 4;
    b |= block->__reserved__ << 5;

    return b;
}