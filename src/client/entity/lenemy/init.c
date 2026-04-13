
//          Sergei Baigerov 2024 - 2026.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/enemy.h>
#include <fightable/entity_library.h>
#include <fightable/state.h>

void _flEnemyInit(struct felenemy* instance) {
	if (!instance) return;

	_fEntityInit(&instance->base);

	instance->base.max_hp = 100;
	instance->base.hp = 100;
	instance->base.global_entity_id = ENTITY_ENEMY;
	instance->base.draw = (void(*)(struct fentity *))_flEnemyDraw;
	instance->base.update = (void(*)(struct fentity *))_flEnemyUpdate;
	instance->base.damage = (void(*)(struct fentity*, float))_flEnemyDamage;
	instance->base.perform_death = (void(*)(struct fentity*))_flEnemyPerformDeath;

	_fEntitySetHitbox(&instance->base, (RLRectangle) { 0, 0, __state.tilemap->tile_size.x, __state.tilemap->tile_size.y });

	instance->base.max_damage_colddown = 0.4f;

	instance->speed_x_mul = (float)GetRandomValue(500, 900) / 1000.f;
	instance->base.max_speed.x *= instance->speed_x_mul;

	_fEntityAddAccessory(&instance->base, ENTITY_ACC_ENEMY_GUN);
}
