
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#define WITH_PLACEHOLDERS

#include <fightable/block.h>
#include <fightable/block_library.h>
#include <fightable/level.h>
#include <fightable/entity.h>
#include <fightable/tilemap.h>

void _fBlockUpdate(struct fblock* block, struct flevel* env) {
    if (!block || !env) return;

    // update dangerous block only if we have loaded entities
    if (block->dangerous && env->entities) {
        // calculate block hitbox
        RLRectangle r = {
            .width = (float)env->tilemap->tile_size.x,
            .height = (float)env->tilemap->tile_size.y,
            .x = (float)(block->base.block_x * env->tilemap->tile_size.x),
            .y = (float)((block->base.block_y) * env->tilemap->tile_size.y - 1),
        };

        // TraceLog(LOG_INFO, "%f %f %f %f", r.x, r.y, r.width, r.height);

        // iterate over all entities
        for (unsigned int i = 0; i < env->entities->len; i++) {
            // get entity
            struct fentity* entity = RSBGetAtIndex_fentity(env->entities, i);
            // check if entity is valid
            if (!entity) continue;

            // get entity hitbox
            RLRectangle er = entity->hitbox;

            // check for collision between entity and block surface
            if (CheckCollisionRecs(er, r)) {
                static const float damage = 1.5f;

                // if entity has a damage handler call it
                if (entity->damage) {
                    entity->damage(entity, damage);
                }
            }
        }
    }
}
