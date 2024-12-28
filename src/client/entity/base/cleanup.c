#include <fightable/entity.h>

void _fEntityCleanup(struct fentity* entity) {
	if (!entity || entity->object_destroyed) return;
	
	if (entity->accessories) {
		RSBDestroy_fentity_accessory(entity->accessories);
	}
}