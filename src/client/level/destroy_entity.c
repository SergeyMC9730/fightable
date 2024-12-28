#include <fightable/level.h>
#include <fightable/entity.h>

void _fLevelDestroyEntity(struct flevel* level, struct fentity* entity) {
	if (!level || !entity || !level->entities) return;

	for (unsigned int i = 0; i < level->entities->added_elements; i++) {
		// WIP
	}
}