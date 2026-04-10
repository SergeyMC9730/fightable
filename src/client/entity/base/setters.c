
//          Sergei Baigerov 2024 - 2026.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/entity.h>

void _fEntitySetHitbox(struct fentity* entity, RLRectangle rec) {
	if (!entity || entity->object_destroyed) return;

	entity->hitbox.hitbox = rec;
	_fEntityUpdateHitbox(&entity->hitbox);
}
void _fEntitySetPosition(struct fentity* entity, Vector2 pos) {
	if (!entity || entity->object_destroyed) return;

	entity->hitbox.hitbox.x = pos.x;
	entity->hitbox.hitbox.y = pos.y;
	_fEntityUpdateHitbox(&entity->hitbox);
}
void _fEntitySetDestroyTimer(struct fentity* entity, float time) {
	if (!entity || entity->object_destroyed) return;

	entity->begin_destruction = 1;
	entity->destroy_timer = time;
}
void _fEntitySetDelta(struct fentity* entity, float delta) {
    if (!entity) return;
    entity->custom_delta = delta;
}

void _fEntityFlipDirection(struct fentity* entity) {
    if (!entity) return;
    entity->render_direction = (entity->render_direction == ENTITY_DIR_LEFT) ? ENTITY_DIR_RIGHT : ENTITY_DIR_LEFT;
}
