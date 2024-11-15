#include <fightable/level.h>
#include <fightable/block.h>
#include <fightable/entity.h>
#include <fightable/block_library.h>

fserializable _fLevelSerialize(struct flevel *level) {
    size_t sz = level->data_size * BLOCK_SIZE + 
                level->entity_data_size * ENTITY_SIZE +
                sizeof(LEVEL_FORMAT_VERSION) +
                sizeof(level->width) + 
                sizeof(level->height) +
                sizeof(level->data_size) +
                sizeof(level->entity_data_size);

    int air_blocks = 0;
    for (int i = 0; i < level->data_size; i++) {
        struct fblock block = level->objects[i];

        if (_fBlockIdFromBlock(block) == BLOCK_AIR) {
            air_blocks++;
        }
    }

    fserializable ret = fCreateSerializableObject(sz);
    fSerializableAddInt16(&ret, LEVEL_FORMAT_VERSION);
    fSerializableAddInt16(&ret, level->width);
    fSerializableAddInt16(&ret, level->height);
    fSerializableAddInt32(&ret, level->data_size - air_blocks);

    for(int i = 0; i < level->data_size - air_blocks; i++) {
        struct fblock lblock = level->objects[i];
        unsigned short id = _fBlockIdFromBlock(lblock);

        if (id == BLOCK_AIR) continue;

        fserializable block = _fBlockSerialize(lblock);
        fSerializableAddData(&ret, block.data, block.size);
        fUnloadSerializableObject(&block);
    }

    fSerializableAddInt32(&ret, level->entity_data_size);

    for(int i = 0; i < level->entity_data_size; i++) {
        fserializable entity = _fEntitySerialize(level->entities[i]);
        fSerializableAddData(&ret, entity.data, entity.size);
        fUnloadSerializableObject(&entity);
    }

    return ret;
}

struct flevel _fLevelLoad(fserializable *serializable) {
    uint16_t level_version = fSerializableGetInt16(serializable);

    struct flevel ret;
    ret.width = fSerializableGetInt16(serializable);
    ret.height = fSerializableGetInt16(serializable);
    ret.data_size = fSerializableGetInt32(serializable);
    ret.objects = MemAlloc(ret.data_size * sizeof(struct fblock));

    for(int i = 0; i < ret.data_size; i++) {
        ret.objects[i] = _fBlockLoad(serializable, level_version);
    }

    ret.entity_data_size = fSerializableGetInt32(serializable);

    for(int i = 0; i < ret.data_size; i++) {
        ret.entities[i] = _fEntityLoad(serializable, level_version);
    }

    return ret;
}
