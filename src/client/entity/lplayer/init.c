#include <fightable/player.h>

void _flPlayerInit(struct felplayer* instance) {
	if (!instance) return;

	_fEntityInit(instance);

	instance->base.global_entity_id = 1;
	instance->base.draw = _flPlayerDraw;
	instance->base.update = _flPlayerUpdate;

	_fEntitySetHitbox(instance, (Rectangle) { 0, 0, 8, 8 });
}