
//          Sergei Baigerov 2024 - 2026.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/entity.h>
#include <fightable/entity_spawner.h>
#include <fightable/ebullet.h>

void _fEntityDoLeftClickAction(struct fentity* entity) {
    if (!entity) return;

    if (_fEntityHasAccessory(entity, ENTITY_ACC_ENEMY_GUN) || _fEntityHasAccessory(entity, ENTITY_ACC_PLAYER_GUN)) {
        switch (entity->global_entity_id) {
            case ENTITY_PLAYER: {
                TraceLog(LOG_INFO, "Trying to spawn player bullet");
                _fEntitySpawnPlayerBullet(entity, BL_STONE);
                break;
            }
            case ENTITY_ENEMY: {
                TraceLog(LOG_INFO, "Trying to spawn enemy bullet");
                _fEntitySpawnEnemyBullet(entity, BL_STONE);
                break;
            }
        }
    }
}
