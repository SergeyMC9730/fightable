
//          Sergei Baigerov 2024 - 2026.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/entity_spawner.h>
#include <fightable/entity.h>
#include <fightable/etext.h>
#include <fightable/level.h>

void _fEntitySpawnTempLabel(struct fentity *instance, char *data) {
    if (!instance) return;

    struct fentity_text* label = (struct fentity_text*)MemAlloc(sizeof(struct fentity_text));
	_feTextInit(label, data);

	label->base.level = instance->level;

	label->base.hitbox.hitbox.x = instance->hitbox.hitbox.x;
	label->base.hitbox.hitbox.y = instance->hitbox.hitbox.y;

	label->base.speed.x = -instance->speed.x * 0.5f;
	label->base.speed.y = -20.f;
	label->base.standing_object = instance->standing_object;
	label->base.complete_nx = instance->complete_nx;
	label->base.complete_px = instance->complete_px;
	label->base.moving_negative = !instance->moving_negative;
	label->base.tint = RED;

	_fEntitySetDestroyTimer(&label->base, 3.f);

	RSB_WRLOCK(label->base.level->entities, RSBAddElement_fentity(label->base.level->entities, &label->base);)
}
