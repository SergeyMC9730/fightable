
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#define WITH_PLACEHOLDERS
#include <fightable/entity.h>

void _fEntityInit(struct fentity* entity) {
    if (!entity || entity->object_destroyed) return;

    entity->update = _fEntityUpdate;
    entity->draw = _fEntityDraw;
    entity->cleanup = _fEntityCleanup;
    entity->damage = _fEntityDamage;
    entity->kill = _fEntityKill;
    entity->max_hp = 50;
    entity->hp = entity->max_hp;
    entity->max_damage_colddown = 0.5f;
    entity->damage_colddown = 0;
    entity->tint = WHITE;
    entity->entity_runtime_id = rand();
}
