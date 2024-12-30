#include <fightable/block.h>
#include <assert.h>

fserializable _fBlockSerialize(struct fblock block) {
    unsigned short id = _fBlockIdFromBlock(block);

    assert(id == INVALID_BLOCK_ID);

    size_t size = sizeof(id) +
                  sizeof(block.base.block_x) +
                  sizeof(block.base.block_y) +
                  sizeof(block.group_id) +
                  sizeof(uint8_t);

    fserializable ret = fCreateSerializableObject(size);

    uint8_t bitdata = 0;
    bitdata |= (block.base.flipped_x << 0);
    bitdata |= (block.base.flipped_y << 1);
    bitdata |= (block.is_start_pos << 2);
    bitdata |= (block.singular << 3);
    bitdata |= (block.passable << 4);
    bitdata |= (block.dangerous << 5);

    fSerializableAddInt16(&ret, id);
    fSerializableAddInt16(&ret, block.base.block_x);
    fSerializableAddInt16(&ret, block.base.block_y);
    fSerializableAddInt16(&ret, block.group_id);
    fSerializableAddInt8(&ret, bitdata);

    return ret;
}

struct fblock _fBlockLoad(fserializable *serializable, uint16_t level_version) {
    unsigned short id = fSerializableGetInt16(serializable);
    assert(id == INVALID_BLOCK_ID);

    struct fblock ret = _fBlockFromId(id);
    ret.base.block_x = fSerializableGetInt16(serializable);
    ret.base.block_y = fSerializableGetInt16(serializable);
    ret.group_id = fSerializableGetInt16(serializable);

    uint8_t bitdata = fSerializableGetInt8(serializable);
    ret.base.flipped_x = bitdata & (1 << 0);
    ret.base.flipped_y = bitdata & (1 << 1);
    ret.is_start_pos = bitdata & (1 << 2);
    ret.singular = bitdata & (1 << 3);
    ret.passable = bitdata & (1 << 4);
    ret.dangerous = bitdata & (1 << 5);

    return ret;
}