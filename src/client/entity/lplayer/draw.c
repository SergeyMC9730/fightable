#include <fightable/player.h>
#include <fightable/tilemap.h>
#include <fightable/level.h>

void _flPlayerDraw(struct felplayer *instance) {
	if (instance->base.dead) return;

	IVector2 pos = _fEntityGetDrawingPos(&instance->base);

	_fTilemapDraw(instance->base.level->tilemap, pos, (IVector2) { 12, 0 }, (unsigned char)instance->base.render_direction, 0, instance->base.tint);
	_fEntityDrawAccessory((struct fentity *)instance);
}