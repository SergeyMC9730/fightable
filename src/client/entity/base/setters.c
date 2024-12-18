#include <fightable/entity.h>

void _fEntitySetHitbox(struct fentity* entity, RLRectangle rec) {
	if (!entity) return;

	entity->hitbox = rec;
}
void _fEntitySetPosition(struct fentity* entity, Vector2 pos) {
	if (!entity) return;

	entity->hitbox.x = pos.x;
	entity->hitbox.y = pos.y;
}