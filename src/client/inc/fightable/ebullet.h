
//          Sergei Baigerov 2024 - 2026.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <fightable/level.h>
#include <fightable/entity.h>

#ifdef __cplusplus
extern "C" {
#endif

enum fentity_bullet_target {
    BT_ENEMY = 0, BT_PLAYER = 1
};
enum fentity_bullet_level {
    BL_STONE = 0, BL_BRONZE = 1
};

struct fentity_bullet {
	struct fentity base;
	enum fentity_bullet_target target;
	enum fentity_bullet_level level;
	rsb_array__fentity *damaged_entities;
	unsigned int entities_damaged_max;
};

void _feBulletInit(struct fentity_bullet* instance, enum fentity_bullet_target target, enum fentity_bullet_level level, enum fentity_direction direction);
void _feBulletDraw(struct fentity_bullet* instance);
void _feBulletCleanup(struct fentity_bullet* instance);
void _feBulletUpdate(struct fentity_bullet* instance);

#ifdef __cplusplus
}
#endif
