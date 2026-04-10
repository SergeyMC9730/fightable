
//          Sergei Baigerov 2024 - 2026.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#define WITH_PLACEHOLDERS

#include <fightable/entity.h>
#include <fightable/debug.h>
#include <fightable/color.h>
#include <fightable/level.h>

// #define DEBUG

void _fEntityUpdate(struct fentity* entity) {
    if (!entity || entity->object_destroyed) return;

    double max_speed_x = entity->max_speed.x;
    double max_speed_y = entity->max_speed.y;

    const double delta = _fEntityGetDelta(entity);

    if (entity->moving_horizontally && !entity->moving_negative) {
        entity->speed.x += delta * 180.f;
        if (entity->speed.x > max_speed_x) {
            entity->speed.x = max_speed_x;
        }
        entity->complete_px = 1;

        entity->render_direction = ENTITY_DIR_RIGHT;
    }
    if (entity->moving_horizontally && entity->moving_negative) {
        entity->speed.x -= delta * 180.f;
        if (entity->speed.x < -max_speed_x) {
            entity->speed.x = -max_speed_x;
        }
        entity->complete_nx = 1;

        entity->render_direction = ENTITY_DIR_LEFT;
    }
    else if (!entity->moving_horizontally) {
        if (entity->complete_nx) {
            entity->speed.x += delta * 350.f;

            if (entity->speed.x > 0) {
                entity->speed.x = 0;
                entity->complete_nx = 0;
            }
        }
        if (entity->complete_px) {
            entity->speed.x -= delta * 350.f;

            if (entity->speed.x < 0) {
                entity->speed.x = 0;
                entity->complete_px = 0;
            }
        }
    }

    if (!entity->on_ground && !entity->no_gravity) {
        entity->speed.y += delta * 100.f;
        if (entity->speed.y > max_speed_y) {
            entity->speed.y = max_speed_y;
        }

        float s = entity->speed.y;

        if (entity->speed.y > 0.f) {
            s *= 1.f;
        }

        _fEntityMove(entity, (Vector2) { 0, s * 3.f });
    }

    _fEntityMove(entity, entity->speed);

    _fEntityUpdateHitbox(&entity->hitbox);

    RLRectangle r1 = entity->hitbox.ground_hitbox;

    entity->on_ground = 0;
    entity->standing_object = (fhitbox){ 0 };

    for (unsigned int i = 0; i < entity->obstacles_length; i++) {
        RLRectangle r2 = entity->obstacles[i];

        if (CheckCollisionRecs(r1, r2)) {
            entity->on_ground = 1;
            entity->standing_object = entity->obstacles[i];
            entity->last_standing_object = entity->standing_object;

            break;
        }
    }

    if (entity->damage_colddown > 0) {
        entity->damage_colddown -= delta;
        if (entity->damage_colddown < 0) entity->damage_colddown = 0;
        entity->tint = _fMixColors(WHITE, RED, entity->damage_colddown * (1 / entity->max_damage_colddown));
    }

    if (entity->begin_destruction) {
        entity->destroy_timer -= delta;
        if (entity->destroy_timer < 0.f) {
            TraceLog(LOG_INFO, "Destroying entity");

            if (entity->cleanup) entity->cleanup(entity);
            else {
                _fEntityCleanup(entity);
            }

            entity->object_destroyed = 1;
            entity->begin_destruction = 0;

            _fLevelDestroyEntity(entity->level, entity);
        }
    }
}

void _fEntityUpdateHitbox(struct fentity_hitbox *env) {
    if (!env) return;

    env->ground_hitbox.width = env->hitbox.width;
    env->ground_hitbox.height = 1;
    env->ground_hitbox.x = env->hitbox.x;
    env->ground_hitbox.y = env->hitbox.y + (env->hitbox.height - env->ground_hitbox.height) + 1;

    if (env->update_unused) {
        env->wall_hitbox_a.width = 2;
        env->wall_hitbox_a.height = env->hitbox.height - 2;
        env->wall_hitbox_a.x = env->hitbox.x - env->wall_hitbox_a.width;
        env->wall_hitbox_a.y = env->hitbox.y + 1;

        env->wall_hitbox_b.width = 2;
        env->wall_hitbox_b.height = env->hitbox.height - 2;
        env->wall_hitbox_b.x = env->hitbox.x + env->hitbox.width;
        env->wall_hitbox_b.y = env->hitbox.y + 1;
    }
}
