#include <fightable/player.h>
#include <fightable/state.h>
#include <fightable/level.h>
#include <stdio.h>

void _flPlayerPerformDeath(struct felplayer* instance) {
	_fAudioFxSlideVolume(&__state.sound_engine, 0.f, 2.f);
	_fLevelTriggerGameOver(instance->base.level);

	printf("player is dead\n");
}