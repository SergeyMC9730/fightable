
//          Sergei Baigerov 2024 - 2026.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/entity.h>
#include <fightable/distance.h>
#include <math.h>

float _fEntityGetDelta(struct fentity* entity) {
    if (!entity || entity->custom_delta <= 0.f) return GetFrameTime();
    return entity->custom_delta;
}

unsigned char _fEntityInWall(struct fentity* entity, struct fentity_hitbox *opt_env) {
    if (!entity->obstacles) return 0;

    struct fentity_hitbox *hitbox = (opt_env) ? opt_env : &entity->hitbox;

    for (unsigned int i = 0; i < entity->obstacles_length; i++) {
        fhitbox ob = entity->obstacles[i];
        if (CheckCollisionRecs(hitbox->wall_hitbox_a, ob)
            || CheckCollisionRecs(hitbox->wall_hitbox_b, ob)
        ) {
            return 1;
        }
    }
    return 0;
}

float _fEntityGetClosestHole(struct fentity *entity) {
    if (!entity) return 0;

    float distance = 96;

    float x_left = distance;
    float x_right = distance;

    struct fentity_hitbox hresult = entity->hitbox;

    Vector2 old_pos = (Vector2){hresult.hitbox.x, hresult.hitbox.y};
    hresult.hitbox.y = entity->last_standing_object.y - 1;

    // get on the left side
    for (float i = 0; i < distance; i++) {
        hresult.hitbox.x--;
        _fEntityUpdateHitbox(&hresult);

        if (_fEntityCanFall(entity, &hresult)) {
            // printf("TRUE. ");
            x_left = -i;
            // printf("(x_left: %f; x1: %f; x2: %f)\n", x_left, entity->hitbox.x, entity->hitbox.x + i);
            break;
        }
    }

    hresult.hitbox.x = old_pos.x;

    // get on the right side
    for (float i = 0; i < distance; i++) {
        hresult.hitbox.x++;
        _fEntityUpdateHitbox(&hresult);

        if (_fEntityCanFall(entity, &hresult)) {
            // printf("TRUE. ");
            x_right = i;
            // printf("(x_right: %f; x1: %f; x2: %f) ", x_left, entity->hitbox.x, entity->hitbox.x - i);
            break;
        }
    }

    // printf("%f %f\n", x_right, x_left);
    return fmin(x_left ,x_right);
}

unsigned char _fEntityCanFall(struct fentity *entity, struct fentity_hitbox *opt_env) {
    if (!entity) return 0;

    struct fentity_hitbox *hitbox = (opt_env) ? opt_env : &entity->hitbox;

    fhitbox r1 = hitbox->ground_hitbox;
    for (unsigned int i = 0; i < entity->obstacles_length; i++) {
        fhitbox r2 = entity->obstacles[i];

        if (CheckCollisionRecs(r1, r2)) {
            return 0;
        }
    }

    return 1;
}

float _fEntityDistanceToEntity(struct fentity *entity_a, struct fentity *entity_b) {
    if (!entity_a || !entity_b) return 0;

    return _fDistPointToPoint(_fHitboxPositionFromHitbox(entity_a->hitbox.hitbox), _fHitboxPositionFromHitbox(entity_b->hitbox.hitbox));
}
