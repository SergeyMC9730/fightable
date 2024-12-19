#include <fightable/entity.h>

void _fEntityCleanup(struct fentity* entity) {
	if (!entity) return;
	
	if (entity->accessories) {
		RSBDestroy_fentity_accessory(entity->accessories);
	}
}