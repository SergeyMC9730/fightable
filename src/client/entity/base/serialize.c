#include <fightable/entity.h>
#include <fightable/serializable.h>

fserializable _fEntitySerialize(struct fentity* entity) {
    fserializable ret = fCreateSerializableObject(ENTITY_SIZE);
    fSerializableAddInt16(&ret, entity->global_entity_id);
    fSerializableAddInt16(&ret, entity->entity_id);
    fSerializableAddFloat(&ret, entity->hitbox.x);
    fSerializableAddFloat(&ret, entity->hitbox.y);

    return ret;
}

struct fentity* _fEntityLoad(fserializable *serializable, uint16_t level_version) {
    struct fentity* ret = MemAlloc(sizeof(struct fentity));
    _fEntityInit(ret);

    ret->global_entity_id = fSerializableGetInt16(serializable);
    ret->entity_id = fSerializableGetInt16(serializable);
    ret->hitbox.x = fSerializableGetFloat(serializable);
    ret->hitbox.y = fSerializableGetFloat(serializable);

    return ret;
}