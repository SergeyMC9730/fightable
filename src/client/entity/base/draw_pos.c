
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/entity.h>
#include <fightable/level.h>

IVector2 _fEntityGetDrawingPos(struct fentity* entity) {
    if (!entity || entity->object_destroyed) return (IVector2) { 0 };

    fhitbox hitbox = entity->hitbox;

    if (entity->level) {
        hitbox.x += entity->level->cam_offset.x;
        hitbox.y += entity->level->cam_offset.y;
    }

    // if ((entity->standing_object.width * entity->standing_object.height) > 0.f) {
    //     hitbox.y = entity->standing_object.y - entity->hitbox.height;
    //     if (entity->level) {
    //         hitbox.y -= entity->level->render_cam.target.y;
    //     }
    // }

    return (IVector2) { (int)hitbox.x, (int)hitbox.y };
}
