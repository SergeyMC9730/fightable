#define WITH_PLACEHOLDERS
#include <fightable/player.h>
#include <fightable/gfx.h>
#include <fightable/state.h>

void _flPlayerDamage(struct felplayer* instance, float hp) {
	_fEntityDamage(&instance->base, hp);
	
	if (instance->base.damage_colddown == instance->base.max_damage_colddown) {
		_fGfxShakeConst(&__state.gfx, 1);
	}
}