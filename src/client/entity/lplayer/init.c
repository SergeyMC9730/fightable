
//          Sergei Baigerov 2024 - 2026.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/player.h>
#include <fightable/entity_library.h>
#include <fightable/state.h>

void _flPlayerInit(struct felplayer* instance) {
	if (!instance) return;

	_fEntityInit(&instance->base);

	instance->base.max_hp = 100;
	instance->base.hp = 100;
	instance->base.global_entity_id = ENTITY_PLAYER;
	instance->base.draw = (void(*)(struct fentity *))_flPlayerDraw;
	instance->base.update = (void(*)(struct fentity *))_flPlayerUpdate;
	instance->base.damage = (void(*)(struct fentity*, float))_flPlayerDamage;
	instance->base.perform_death = (void(*)(struct fentity*))_flPlayerPerformDeath;
	instance->regen_ticks = 0;
	instance->regen_ticks_max = 100;

	_fEntitySetHitbox(&instance->base, (RLRectangle) { 0, 0, __state.tilemap->tile_size.x, __state.tilemap->tile_size.y });

	instance->base.max_damage_colddown = 0.4f;

	_fEntityAddAccessory(&instance->base, ENTITY_ACC_PLAYER_GUN);
}
