#define WITH_PLACEHOLDERS

#include <fightable/block.h>
#include <fightable/block_library.h>
#include <fightable/level.h>
#include <fightable/entity.h>
#include <fightable/tilemap.h>
#include <stdio.h>

void _fBlockUpdate(struct fblock* block, struct flevel* env) {
	if (!block || !env) return;
	
	unsigned short id = _fBlockIdFromBlock(*block);
	switch (id) {
	case BLOCK_MAGMA: {
		if (env->entities == NULL) break;

		RLRectangle r = {
			.width = (float)env->tilemap->tile_size.x,
			.height = (float)env->tilemap->tile_size.y,
			.x = (float)(block->base.block_x * env->tilemap->tile_size.x),
			.y = (float)((block->base.block_y - 1) * env->tilemap->tile_size.y),
		};

		for (unsigned int i = 0; i < env->entity_data_size; i++) {
			struct fentity* entity = env->entities[i];

			RLRectangle er = entity->hitbox;
			IVector2 v = _fEntityGetDrawingPos(entity);
			er.x = (float)v.x;
			er.y = (float)v.y;

			if (CheckCollisionRecs(er, r)) {
				static const float damage = 1.5f;

				if (entity->damage) entity->damage(entity, damage);
				else {
					_fEntityDamage(entity, damage);
				}
				if (entity->damage_colddown == entity->max_damage_colddown) {
					printf("damage!\n");
				}
			}
		}
	}
	}
}