#include <fightable/player.h>

void _flPlayerDraw(void* _instance) {
	struct felplayer* instance = (struct felplayer*)_instance;
	IVector2 pos = _fEntityGetDrawingPos(instance);

	DrawRectangle(pos.x, pos.y, 8, 8, WHITE);
}