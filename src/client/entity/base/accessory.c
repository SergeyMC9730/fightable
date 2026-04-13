
//          Sergei Baigerov 2024 - 2026.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/entity.h>
#include <fightable/tilemap.h>
#include <fightable/level.h>
#include <fightable/compile_config.h>
#include <fightable/ebullet_render_props.h>

#ifdef TARGET_SUPPORTS_OVERLAY
#include <fightable/renderer.h>

void _fEntityAccessoryGunOverlayDrawer(void *ctx) {
    struct fentity* entity = (struct fentity *)ctx;

    float scaling = (float)UI_SCALE;
    float scaling2 = scaling * EBULLET_SCALING;

    IVector2 basepos = _fEntityGetDrawingPos(entity);
    IVector2 pos = {
        .x = (int)((float)(basepos.x + (int)(entity->hitbox.hitbox.width)) * scaling),
        .y = (int)((float)basepos.y * scaling)
    };

    if (entity->render_direction == ENTITY_DIR_LEFT) {
        pos.x = (basepos.x * scaling) - (int)((entity->hitbox.hitbox.width / 1.5f) * scaling);
        _fTilemapDrawScaled(entity->level->tilemap, pos, (IVector2){0, 6}, 1, 0, entity->tint, scaling2);
        pos.x -= entity->level->tilemap->tile_size.x * scaling2;
        _fTilemapDrawScaled(entity->level->tilemap, pos, (IVector2){1, 6}, 1, 0, entity->tint, scaling2);
    } else {
        _fTilemapDrawScaled(entity->level->tilemap, pos, (IVector2){0, 6}, 0, 0, entity->tint, scaling2);
        pos.x += entity->level->tilemap->tile_size.x * scaling2;
        _fTilemapDrawScaled(entity->level->tilemap, pos, (IVector2){1, 6}, 0, 0, entity->tint, scaling2);
    }
}
#endif

RSB_ARRAY_IMPL_GEN(struct fentity_accessory, _fentity_accessory);

void _fEntityDrawAccessory(struct fentity* entity) {
    if (!entity || !entity->accessories || entity->object_destroyed) return;

    IVector2 basepos = _fEntityGetDrawingPos(entity);

    for (unsigned int i = 0; i < entity->accessories->len; i++) {
        struct fentity_accessory a = RSBGetAtIndex_fentity_accessory(entity->accessories, i);

        switch(a.type) {
        case ENTITY_ACC_HAT_1: {
            IVector2 pos = {
                .x = basepos.x - 1,
                .y = basepos.y - (int)(entity->hitbox.hitbox.height - 2)
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
                .y = basepos.y - (int)entity->hitbox.hitbox.height
            };

            _fTilemapDraw(entity->level->tilemap, pos, (IVector2) { 1, 7 }, entity->render_direction, 0, entity->tint);

            break;
        }
        case ENTITY_ACC_HAT_3: {
            IVector2 pos = {
                .x = basepos.x,
                .y = basepos.y - (int)entity->hitbox.hitbox.height
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
        case ENTITY_ACC_ENEMY_GUN:
        case ENTITY_ACC_PLAYER_GUN: {
            IVector2 pos = {
                .x = basepos.x + (int)(entity->hitbox.hitbox.width / 1.5f),
                .y = basepos.y
            };

#ifndef TARGET_SUPPORTS_OVERLAY
            if (entity->render_direction == ENTITY_DIR_LEFT) {
                pos.x = basepos.x - (int)(entity->hitbox.hitbox.width / 1.5f);
                _fTilemapDraw(entity->level->tilemap, pos, (IVector2){0, 6}, 1, 0, entity->tint);
                pos.x -= entity->level->tilemap->tile_size.x;
                _fTilemapDraw(entity->level->tilemap, pos, (IVector2){1, 6}, 1, 0, entity->tint);
            } else {
                _fTilemapDraw(entity->level->tilemap, pos, (IVector2){0, 6}, 0, 0, entity->tint);
                pos.x += entity->level->tilemap->tile_size.x;
                _fTilemapDraw(entity->level->tilemap, pos, (IVector2){1, 6}, 0, 0, entity->tint);
            }
#else
            renderer_event_t event;
            event.callback = _fEntityAccessoryGunOverlayDrawer;
            event.user = entity;
            _fScheduleOverlayFunc(event);
#endif
            //_fTilemapDrawMegatile(entity->level->tilemap, pos, (IVector2){0, 6}, (IVector2){2, 1}, entity->render_direction, 0, entity->tint);

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

unsigned char _fEntityHasAccessory(struct fentity* entity, enum fentity_accessory_obj accessory) {
    if (!entity) return 0;

    if (entity->accessories) {
        RSB_RDLOCK_BEGIN(entity->accessories);
        if (accessory == ENTITY_ACC_NONE && entity->accessories->len == 0) {
            RSB_RDLOCK_END(entity->accessories);
            return 1;
        }

        for (unsigned int i = 0; i < entity->accessories->len; i++) {
            struct fentity_accessory a = entity->accessories->objects[i];
            if (a.type == accessory) {
                RSB_RDLOCK_END(entity->accessories);
                return 1;
            }
        }

        RSB_RDLOCK_END(entity->accessories);
        return 0;
    }

    return 0;
}
