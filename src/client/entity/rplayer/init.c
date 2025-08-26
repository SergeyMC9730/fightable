
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/rplayer.h>
#include <fightable/entity_library.h>

void _frPlayerInit(struct ferplayer* instance) {
	if (!instance) return;

	_fEntityInit(&instance->base);

	instance->base.global_entity_id = ENTITY_REMOTE_PLAYER;
	instance->base.draw = (void(*)(struct fentity *))_frPlayerDraw;
	instance->base.update = (void(*)(struct fentity *))_frPlayerUpdate;

	_fEntitySetHitbox(&instance->base, (RLRectangle) { 0, 0, 8, 8 });

	instance->base.max_damage_colddown = 0.4f;
}
