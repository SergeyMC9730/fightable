#include <fightable/player.h>

void _flPlayerDraw(struct felplayer *instance) {
	IVector2 pos = _fEntityGetDrawingPos(&instance->base);

	DrawRectangle(pos.x, pos.y, 8, 8, WHITE);
}