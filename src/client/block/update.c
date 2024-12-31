#define WITH_PLACEHOLDERS

#include <fightable/block.h>
#include <fightable/block_library.h>
#include <fightable/level.h>
#include <fightable/entity.h>
#include <fightable/tilemap.h>
#include <stdio.h>

void _fBlockUpdate(struct fblock* block, struct flevel* env) {
	if (!block || !env) return;

	if (block->dangerous && env->entities) {
		RLRectangle r = {
			.width = (float)env->tilemap->tile_size.x,
			.height = (float)env->tilemap->tile_size.y,
			.x = (float)(block->base.block_x * env->tilemap->tile_size.x),
			.y = (float)((block->base.block_y) * env->tilemap->tile_size.y - 1),
		};

		for (unsigned int i = 0; i < env->entities->added_elements; i++) {
			struct fentity* entity = RSBGetAtIndex_fentity(env->entities, i);
			if (!entity) continue;

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
			}
		}
	}
}