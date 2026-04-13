
//          Sergei Baigerov 2024 - 2026.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/entity.h>

struct felenemy {
	struct fentity base;
	struct fentity *followed_entity;
	float speed_x_mul;
	float left_click_timeout;
};

#ifdef __cplusplus
extern "C" {
#endif

void _flEnemyInit(struct felenemy* instance);
void _flEnemyDraw(struct felenemy* instance);
void _flEnemyUpdate(struct felenemy* instance);
void _flEnemyDamage(struct felenemy* instance, float hp);
void _flEnemyPerformDeath(struct felenemy* instance);

#ifdef __cplusplus
}
#endif
