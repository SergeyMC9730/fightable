#define WITH_PLACEHOLDERS
#include <fightable/player.h>
#include <fightable/gfx.h>
#include <fightable/state.h>
#include <fightable/gfx.h>

void _flPlayerDamage(struct felplayer* instance, float hp) {
	_fEntityDamage(&instance->base, hp);
	
	if (instance->base.damage_colddown == instance->base.max_damage_colddown) {
		TraceLog(LOG_INFO, "Applied damage to local player");

		_fGfxShakeConst(&__state.gfx, 1.2);

		if (!instance->base.dead) {
			const float old_vol = __state.config.volume_slider.progress;

			_fAudioSetVolume(&__state.sound_engine, old_vol / 4);
			_fAudioFxSlideVolume(&__state.sound_engine, old_vol, 0.5f);

			_fGfxActivateDamageOverlay();
			_ntRendererResetAnimation(__state.damage_overlay_anim);
		}
		else {
			if (__state.damage_overlay_play) {
				__state.damage_overlay_timer = 1.f;
			}
		}
	}
}