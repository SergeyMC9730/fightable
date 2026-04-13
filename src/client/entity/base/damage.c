
//          Sergei Baigerov 2024 - 2026.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#define WITH_PLACEHOLDERS

#include <fightable/entity.h>
#include <fightable/level.h>

void _fEntityDamage(struct fentity* entity, float damage) {
	if (!entity || entity->dead || entity->damage_colddown > 0 || entity->object_destroyed) return;

	entity->hp -= damage;
	if (entity->hp <= 0) {
		entity->dead = 1;
		entity->hp = 0;

		if (entity->perform_death) entity->perform_death(entity);
	}

	entity->damage_colddown = entity->max_damage_colddown;
}

void _fEntityKill(struct fentity* entity) {
	if (!entity || entity->object_destroyed) return;

	entity->damage(entity, entity->max_hp);
}

void _fEntityDestroy(struct fentity* entity) {
    if (!entity || entity->object_destroyed) return;

    if (entity->cleanup) entity->cleanup(entity);
    else {
        _fEntityCleanup(entity);
    }

    entity->object_destroyed = 1;
    entity->begin_destruction = 0;

    _fLevelDestroyEntity(entity->level, entity);
}
