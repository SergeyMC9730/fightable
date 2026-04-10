
//          Sergei Baigerov 2024 - 2026.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#define WITH_PLACEHOLDERS
#include <fightable/enemy.h>
#include <fightable/level.h>

void _flEnemyUpdate(struct felenemy* instance) {
    struct fentity* entity = &instance->base;

    unsigned char idle = !instance->followed_entity || instance->followed_entity == entity;

    entity->moving_horizontally = 0;
    entity->moving_negative = 0;

    if (entity->dead) {
        _fEntityUpdate(entity);
        return;
    }

    if (entity->doesnt_move && idle) {
        _fEntityFlipDirection(entity);
    }

    if (!idle) {
        fhitbox epos = instance->followed_entity->hitbox.hitbox;

        entity->render_direction = ENTITY_DIR_LEFT;
        if (epos.x >= entity->hitbox.hitbox.x) {
            entity->render_direction = ENTITY_DIR_RIGHT;
        }
    }

    if (entity->render_direction == ENTITY_DIR_LEFT) {
        entity->moving_horizontally = 1;
        entity->moving_negative = 1;
    } else {
        entity->moving_horizontally = 1;
        entity->moving_negative = 0;
    }

    _fEntityUpdate(entity);

    float hole = _fEntityGetClosestHole(entity);
    float max_dist = 13.f;
    unsigned char should_jump =
        (hole >= -max_dist && entity->render_direction == ENTITY_DIR_LEFT)
        ||
        (hole <= max_dist && entity->render_direction == ENTITY_DIR_RIGHT);

    if (should_jump) {
        if (idle) {
            _fEntityFlipDirection(entity);
        } else {
            _fEntityJump(entity, 1);
        }
    }

    if (entity->level) {
        struct fentity *player = _fLevelFindPlayer(entity->level);
        if (player) {
            float dist = _fEntityDistanceToEntity(entity, player);
            if (dist <= (64.f / instance->speed_x_mul)) {
                instance->followed_entity = player;
            } else {
                instance->followed_entity = NULL;
            }
        }
    }
}
