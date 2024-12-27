#include <fightable/level.h>

void _fLevelTriggerGameOver(struct flevel* level) {
	level->in_gameover_mode = 1;
	level->gameover_time = 0;
}