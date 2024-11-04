#include <fightable/player.h>
#include <fightable/entity_library.h>

void _flPlayerInit(struct felplayer* instance) {
	if (!instance) return;

	_fEntityInit(&instance->base);

	instance->base.global_entity_id = ENTITY_PLAYER;
	instance->base.draw = (void(*)(struct fentity *))_flPlayerDraw;
	instance->base.update = (void(*)(struct fentity *))_flPlayerUpdate;

	_fEntitySetHitbox(&instance->base, (Rectangle) { 0, 0, 8, 8 });
}