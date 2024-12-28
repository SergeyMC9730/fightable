#include <fightable/entity.h>

void _fEntitySetHitbox(struct fentity* entity, RLRectangle rec) {
	if (!entity || entity->object_destroyed) return;

	entity->hitbox = rec;
}
void _fEntitySetPosition(struct fentity* entity, Vector2 pos) {
	if (!entity || entity->object_destroyed) return;

	entity->hitbox.x = pos.x;
	entity->hitbox.y = pos.y;
}
void _fEntitySetDestroyTimer(struct fentity* entity, float time) {
	if (!entity || entity->object_destroyed) return;

	entity->begin_destruction = 1;
	entity->destroy_timer = time;
}