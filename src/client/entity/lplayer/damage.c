#define WITH_PLACEHOLDERS
#include <fightable/player.h>
#include <fightable/gfx.h>
#include <fightable/state.h>
#include <fightable/etext.h>
#include <fightable/level.h>
#include <stdio.h>

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

		TraceLog(LOG_INFO, "Spawning label \"%d\"", (int)hp);

		char buffer[16] = {};
		snprintf(buffer, 16, "%d", (int)hp);

		struct fentity_text* label = (struct fentity_text*)MemAlloc(sizeof(struct fentity_text));
		_feTextInit(label, buffer);

		label->base.level = instance->base.level;

		label->base.hitbox.x = instance->base.hitbox.x;
		label->base.hitbox.y = instance->base.hitbox.y;

		label->base.speed.x = -instance->base.speed.x * 0.5f;
		label->base.speed.y = -20.f;
		label->base.standing_object = instance->base.standing_object;
		label->base.complete_nx = instance->base.complete_nx;
		label->base.complete_px = instance->base.complete_px;
		label->base.moving_negative = !instance->base.moving_negative;
		label->base.tint = RED;

		_fEntitySetDestroyTimer(&label->base, 3.f);

		RSBAddElement_fentity(label->base.level->entities, &label->base);
	}
}