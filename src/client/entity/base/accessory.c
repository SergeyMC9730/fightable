#include <fightable/entity.h>
#include <fightable/tilemap.h>
#include <fightable/level.h>

RSB_ARRAY_IMPL_GEN(struct fentity_accessory, _fentity_accessory);

void _fEntityDrawAccessory(struct fentity* entity) {
	if (!entity || !entity->accessories) return;

	IVector2 basepos = _fEntityGetDrawingPos(entity);
	
	for (unsigned int i = 0; i < entity->accessories->len; i++) {
		struct fentity_accessory a = RSBGetAtIndex_fentity_accessory(entity->accessories, i);

		switch(a.type) {
		case ENTITY_ACC_HAT_1: {
			IVector2 pos = {
				.x = basepos.x - 1,
				.y = basepos.y - (int)(entity->hitbox.height - 2)
			};

			if (entity->render_direction == ENTITY_DIR_LEFT) {
				pos.x = basepos.x + 1;
			}

			_fTilemapDraw(entity->level->tilemap, pos, (IVector2) { 11, 1 }, entity->render_direction, 0, entity->tint);

			break;
		}
		case ENTITY_ACC_HAT_2: {
			IVector2 pos = {
				.x = basepos.x,
				.y = basepos.y - (int)entity->hitbox.height
			};

			_fTilemapDraw(entity->level->tilemap, pos, (IVector2) { 1, 7 }, entity->render_direction, 0, entity->tint);

			break;
		}
		case ENTITY_ACC_HAT_3: {
			IVector2 pos = {
				.x = basepos.x,
				.y = basepos.y - (int)entity->hitbox.height
			};

			_fTilemapDraw(entity->level->tilemap, pos, (IVector2) { 2, 7 }, entity->render_direction, 0, entity->tint);

			break;
		}
		case ENTITY_ACC_GLASSES_1: {
			IVector2 pos = {
				.x = basepos.x,
				.y = basepos.y
			};

			_fTilemapDraw(entity->level->tilemap, pos, (IVector2) { 0, 7 }, entity->render_direction, 0, entity->tint);

			break;
		}
		}
	}
}

void _fEntityAddAccessory(struct fentity* entity, enum fentity_accessory_obj accessory) {
	if (!entity || accessory == ENTITY_ACC_NONE) return;
	if (!entity->accessories) entity->accessories = RSBCreateArray_fentity_accessory();

	struct fentity_accessory a = {
		.type = (unsigned char)accessory
	};

	RSBAddElement_fentity_accessory(entity->accessories, a);
}