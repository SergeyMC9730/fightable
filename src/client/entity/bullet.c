
//          Sergei Baigerov 2024 - 2026.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "raylib.h"
#define WITH_PLACEHOLDERS
#include <fightable/ebullet.h>
#include <fightable/state.h>
#include <fightable/entity_library.h>
#include <fightable/compile_config.h>
#include <fightable/level.h>

void _feBulletInit(struct fentity_bullet* instance, enum fentity_bullet_target target, enum fentity_bullet_level level, enum fentity_direction direction) {
    if (instance == NULL) return;

    _fEntityInit(&instance->base);

    instance->level = level;
    instance->target = target;
    instance->damaged_entities = RSBCreateArray_fentity();
    instance->base.render_direction = direction;
    instance->base.no_gravity = 1;
    instance->base.max_speed.x = 1.5f / 1.1f * 140.f;
    instance->base.accel_speed.x = 1000.f;

    switch (level) {
        case BL_STONE: {
            instance->entities_damaged_max = 2;
            break;
        }
        case BL_BRONZE: {
            instance->entities_damaged_max = 4;
            break;
        }
    }

    fhitbox hitbox = instance->base.hitbox.hitbox;
    hitbox.width = 8; hitbox.height = 3;

    _fEntitySetHitbox(&instance->base, hitbox);
    _fEntitySetDestroyTimer(&instance->base, 30.f);
    instance->base.global_entity_id = ENTITY_BULLET;
    instance->base.draw = (void(*)(struct fentity*))_feBulletDraw;
    instance->base.update = (void(*)(struct fentity*))_feBulletUpdate;
    instance->base.cleanup = (void(*)(struct fentity*))_feBulletCleanup;
}
void _feBulletDraw(struct fentity_bullet* instance) {
    if (!instance) return;

    IVector2 pos = _fEntityGetDrawingPos(&instance->base);
    IVector2 bullet_tile;

    switch (instance->level) {
        case BL_STONE: {
            bullet_tile = (IVector2){2, 6};
            break;
        }
        case BL_BRONZE: {
            bullet_tile = (IVector2){3, 6};
            break;
        }
    }

    _fTilemapDraw(instance->base.level->tilemap, pos, bullet_tile, instance->base.render_direction, 0, instance->base.tint);
}
void _feBulletCleanup(struct fentity_bullet* instance) {
    if (!instance) return;

    if (instance->damaged_entities) {
        RSBDestroy_fentity(instance->damaged_entities);
    }
}

unsigned char _feBulletDamagedEntity(struct fentity_bullet* instance, struct fentity *entity) {
    if (!instance || !entity || !instance->damaged_entities) return 0;

    for (unsigned int i = 0; i < instance->damaged_entities->len; i++) {
        struct fentity* l_entity = RSBGetAtIndex_fentity(instance->damaged_entities, i);
        if (l_entity == entity) return 1;
    }

    return 0;
}

void _feBulletUpdate(struct fentity_bullet* instance) {
    if (!instance) return;

    struct fentity* entity = &instance->base;

    if (entity->render_direction == ENTITY_DIR_LEFT) {
        entity->moving_horizontally = 1;
        entity->moving_negative = 1;
    } else {
        entity->moving_horizontally = 1;
        entity->moving_negative = 0;
    }

    _fEntityUpdate(entity);

    if (entity->doesnt_move) {
        _fEntityDestroy(entity);

        return;
    }

    unsigned int len = entity->level->entities->len;
    for (unsigned int i = 0; i < len; i++) {
        RSB_RDLOCK(entity->level->entities, struct fentity* l_entity = RSBGetAtIndex_fentity(entity->level->entities, i););
        if (!l_entity || l_entity == entity) continue;

        if (!CheckCollisionRecs(entity->hitbox.hitbox, l_entity->hitbox.hitbox)) continue;
        if (_feBulletDamagedEntity(instance, l_entity)) continue;

        if (l_entity->global_entity_id == ENTITY_ENEMY && instance->target == BT_ENEMY) {
            RSBAddElement_fentity(instance->damaged_entities, l_entity);
            l_entity->damage(l_entity, 10.f);
        } else if (l_entity->global_entity_id == ENTITY_PLAYER && instance->target == BT_PLAYER) {
            RSBAddElement_fentity(instance->damaged_entities, l_entity);
            l_entity->damage(l_entity, 10.f);
        }
    }

    if (instance->damaged_entities->len > instance->entities_damaged_max) {
        _fEntityDestroy(entity);
    }
}
