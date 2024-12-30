#include <fightable/level.h>
#include <fightable/block.h>
#include <fightable/entity.h>
#include <fightable/block_library.h>
#include <stdio.h>

fserializable _fLevelSerialize(struct flevel *level) {
    unsigned int entities = 0;
    if (level->entities) {
        entities = level->entities->added_elements;
    }

    size_t sz = level->data_size * BLOCK_SIZE + 
                entities * ENTITY_SIZE +
                sizeof(LEVEL_FORMAT_VERSION) +
                sizeof(level->width) + 
                sizeof(level->height) +
                sizeof(level->data_size) +
                sizeof(entities);

    printf("size=%d; blocks=%d; w=%d; h=%d; e=%d\n", sz, level->data_size, level->width, level->height, entities);

    int air_blocks = 0;
    for (int i = 0; i < level->data_size; i++) {
        struct fblock block = level->objects[i];

        if (_fBlockIdFromBlock(block) == BLOCK_AIR) {
            air_blocks++;
        }
    }

    printf("%d of air blocks\n", air_blocks);

    fserializable ret = fCreateSerializableObject(sz);
    fSerializableAddInt16(&ret, LEVEL_FORMAT_VERSION);
    fSerializableAddInt16(&ret, level->width);
    fSerializableAddInt16(&ret, level->height);
    fSerializableAddInt32(&ret, level->data_size - air_blocks);

    printf("added 4 elements\n");

    for(int i = 0; i < level->data_size - air_blocks; i++) {
        struct fblock lblock = level->objects[i];
        unsigned short id = _fBlockIdFromBlock(lblock);

        if (id == BLOCK_AIR) continue;

        fserializable block = _fBlockSerialize(lblock);
        printf("serialized block\n");

        fSerializableAddData(&ret, block.data, block.size);

        printf("added block with id %d\n", id);

        fUnloadSerializableObject(&block);
    }

    printf("adding entities (amount=%d)\n", entities);

    fSerializableAddInt32(&ret, entities);

    for(int i = 0; i < entities; i++) {
        fserializable entity = _fEntitySerialize(level->entities->objects[i]);
        fSerializableAddData(&ret, entity.data, entity.size);
        fUnloadSerializableObject(&entity);
    }

    printf("done\n");

    return ret;
}

struct flevel *_fLevelLoad(fserializable *serializable) {
    uint16_t level_version = fSerializableGetInt16(serializable);

    struct flevel* ret = (struct flevel*)MemAlloc(sizeof(struct flevel));
    ret->width = fSerializableGetInt16(serializable);
    ret->height = fSerializableGetInt16(serializable);
    ret->data_size = fSerializableGetInt32(serializable);
    ret->objects = (struct fblock *)MemAlloc(ret->data_size * sizeof(struct fblock));

    for(int i = 0; i < ret->data_size; i++) {
        ret->objects[i] = _fBlockLoad(serializable, level_version);
    }

    int amount = fSerializableGetInt32(serializable);
    if (amount > 0) {
        ret->entities = RSBCreateArray_fentity();

        for (int i = 0; i < amount; i++) {
            RSBAddElement_fentity(ret->entities, _fEntityLoad(serializable, level_version));
        }
    }

    return ret;
}
