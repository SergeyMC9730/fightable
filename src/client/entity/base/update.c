#include <fightable/entity.h>
#include <raylib.h>
#include <stdio.h>
#include <fightable/debug.h>
#include <fightable/color.h>

#define X_SPEED 70.f
#define Y_SPEED 150.f

// #define DEBUG

void _fEntityUpdate(struct fentity* entity) {
    static const double max_speed_x = 1.5 / 1.1 * 70.f;
    static const double max_speed_y = 3.f * 20.f;

    const double delta = GetFrameTime();

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

    if (!entity->on_ground) {
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

    entity->ground_hitbox.width = entity->hitbox.width;
    entity->ground_hitbox.height = 1;
    entity->ground_hitbox.x = entity->hitbox.x;
    entity->ground_hitbox.y = entity->hitbox.y + (entity->hitbox.height - entity->ground_hitbox.height) + 1;

    RLRectangle r1 = *(RLRectangle*)(&entity->ground_hitbox);

    entity->on_ground = 0;
    entity->standing_object = (fhitbox){ 0 };

    for (unsigned int i = 0; i < entity->obstacles_length; i++) {
        RLRectangle r2 = *(RLRectangle*)(entity->obstacles + i);

        if (CheckCollisionRecs(r1, r2)) {
            entity->on_ground = 1;
            entity->standing_object = entity->obstacles[i];

            break;
        }
    }

    if (entity->damage_colddown > 0) {
        entity->damage_colddown -= GetFrameTime();
        if (entity->damage_colddown < 0) entity->damage_colddown = 0;
        entity->tint = _fMixColors(WHITE, RED, entity->damage_colddown * (1 / entity->max_damage_colddown));
    }
}