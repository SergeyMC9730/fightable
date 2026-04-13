
//          Sergei Baigerov 2024 - 2026.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/entity.h>
#include <fightable/entity_spawner.h>

void _fEntityHeal(struct fentity* entity, float hp, unsigned char show_text) {
    if (!entity) return;

    int old_hp = (int)entity->hp;

    entity->hp += hp;
    if (entity->hp > entity->max_hp) {
        entity->hp = entity->max_hp;
    }

    int delta_hp = (int)entity->hp - old_hp;

    if (show_text) {
        char buffer[16] = {};
		snprintf(buffer, 16, "<cgreen>%d", (int)delta_hp);

		struct fentity *text = _fEntitySpawnTempLabel(entity, buffer);
		text->tint = GREEN;
		text->speed.y *= 1.5f;
    }
}
