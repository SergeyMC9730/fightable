
//          Sergei Baigerov 2024 - 2026.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#define WITH_PLACEHOLDERS
#include <fightable/player.h>
#include <fightable/renderer.h>
#include <fightable/level.h>

void _flPlayerUpdate(struct felplayer* instance) {
    struct fentity* entity = &instance->base;

    entity->moving_horizontally = 0;
    entity->moving_negative = 0;

    if (!entity->dead) {
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
            entity->moving_horizontally = 1;
            entity->moving_negative = 0;
        }
        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
            entity->moving_horizontally = 1;
            entity->moving_negative = 1;
        }
        if ((IsKeyDown(KEY_W) || IsKeyDown(KEY_SPACE))) {
            _fEntityJump(entity, 1);
        }
        if ((IsKeyPressed(KEY_W) || IsKeyPressed(KEY_SPACE))) {
            _fEntityJump(entity, 0);
        }
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mouse = _fGetMousePosPix();
            Vector2 fb = _fGetCurrentFramebufferSize();
            if (mouse.x < (fb.x / 2)) {
                entity->render_direction = ENTITY_DIR_LEFT;
            } else {
                entity->render_direction = ENTITY_DIR_RIGHT;
            }

            _fEntityDoLeftClickAction(entity);
        }
    }

    if (entity->level) {
        instance->regen_ticks_max = entity->level->tps * 3;
    }

    instance->regen_ticks++;
    if (instance->regen_ticks == instance->regen_ticks_max) {
        if (entity->hp != entity->max_hp) {
             _fEntityHeal(entity, 2.5f, 1);
        }
        instance->regen_ticks = 0;
    }

    _fEntityUpdate(entity);
}
