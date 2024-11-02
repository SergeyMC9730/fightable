#include <fightable/player.h>

void _flPlayerDraw(struct felplayer* instance) {
	IVector2 pos = _fEntityGetDrawingPos(instance);

	DrawRectangle(pos.x, pos.y, 8, 8, WHITE);
}