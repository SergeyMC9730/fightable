
//          Sergei Baigerov 2024 - 2026.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#define WITH_PLACEHOLDERS
#include <fightable/enemy.h>
#include <fightable/gfx.h>
#include <fightable/state.h>
#include <fightable/entity_spawner.h>
#include <fightable/level.h>
#include <stdio.h>

void _flEnemyDamage(struct felenemy* instance, float hp) {
    if (!instance || instance->base.damage_colddown > 0) return;

    int old_hp = (int)instance->base.hp;
	_fEntityDamage(&instance->base, hp);
	int delta_hp = old_hp - (int)instance->base.hp;

	if (instance->base.damage_colddown == instance->base.max_damage_colddown) {
		TraceLog(LOG_INFO, "enemy: Spawning label \"%d\"", (int)delta_hp);

		char buffer[16] = {};
		snprintf(buffer, 16, "%d", (int)delta_hp);

		_fEntitySpawnTempLabel(&instance->base, buffer);
	}
}
