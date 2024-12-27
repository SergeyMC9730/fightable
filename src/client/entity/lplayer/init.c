#include <fightable/player.h>
#include <fightable/entity_library.h>

void _flPlayerInit(struct felplayer* instance) {
	if (!instance) return;

	_fEntityInit(&instance->base);

	instance->base.global_entity_id = ENTITY_PLAYER;
	instance->base.draw = (void(*)(struct fentity *))_flPlayerDraw;
	instance->base.update = (void(*)(struct fentity *))_flPlayerUpdate;
	instance->base.damage = (void(*)(struct fentity*, float))_flPlayerDamage;
	instance->base.perform_death = (void(*)(struct fentity*))_flPlayerPerformDeath;

	_fEntitySetHitbox(&instance->base, (RLRectangle) { 0, 0, 8, 8 });

	instance->base.max_damage_colddown = 0.4f;
}