#include <fightable/level.h>
#include <fightable/state.h>

void _fLevelTriggerGameOver(struct flevel* level) {
	level->in_gameover_mode = 1;
	level->gameover_time = 0;

	_fAudioFxSlideVolume(&__state.sound_engine, 0.f, 1.f);
}