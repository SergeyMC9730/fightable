
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/entity.h>
#include <fightable/color.h>

void _fEntityDraw(struct fentity *entity) {
    if (!entity || entity->dead || entity->object_destroyed) return;

    fhitbox hitbox = entity->hitbox;
    IVector2 pos = _fEntityGetDrawingPos(entity);

    hitbox.x = pos.x;
    hitbox.y = pos.y;

    Color col = BLUE;
    if (entity->damage_colddown != 0.f) {
        _fMixColors(BLUE, entity->tint, entity->damage_colddown * (1 / entity->max_damage_colddown));
    }
    DrawRectangle((int)hitbox.x, (int)hitbox.y, (int)hitbox.width, (int)hitbox.height, col);

    _fEntityDrawAccessory(entity);
}
