#include <fightable/player.h>
#include <fightable/level.h>
#include <stdio.h>

void _flPlayerPerformDeath(struct felplayer* instance) {
	_fLevelTriggerGameOver(instance->base.level);

	TraceLog(LOG_INFO, "Player has died");
}