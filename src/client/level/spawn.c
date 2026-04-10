
//          Sergei Baigerov 2024 - 2026.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/level.h>
#include <fightable/state.h>
#include <fightable/editor.h>
#include <fightable/player.h>
#include <fightable/block_library.h>

void _fLevelPerformBasicSpawn(struct flevel *level, unsigned char full_reset) {
    level->pause_world = 1;

    IVector2 pos = _fEditorGetPosOfFirstIdLvl(level, BLOCK_START);
    TraceLog(LOG_INFO, "pos=%d %d", pos.x, pos.y);

    if (level->hitboxes) {
        free(level->hitboxes);
    }
    level->hitboxes = _fLevelGetHitboxes(level);

    struct felplayer* player = (struct felplayer*)MemAlloc(sizeof(struct felplayer));

    player->base.level = level;

    _flPlayerInit(player);
    _fEntitySetPosition(&player->base, (Vector2){ pos.x * player->base.hitbox.hitbox.width, pos.y * player->base.hitbox.hitbox.height });
    _fEntityAddAccessory(&player->base, ENTITY_ACC_HAT_2);
    _fEntityAddAccessory(&player->base, ENTITY_ACC_GLASSES_1);

    if (full_reset) {
        if (level->entities) {
            for (unsigned long i = 0; i < level->entities->len; i++) {
                struct fentity *e = level->entities->objects[i];
                e->cleanup(e);
            }
            RSBDestroy_fentity(level->entities);
        }
    }

    if (!level->entities) {
        level->entities = RSBCreateArray_fentity();
    }
    RSBAddElement_fentity(level->entities, &player->base);

    level->pause_world = 0;
}
