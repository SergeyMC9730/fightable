
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <fightable/entity.h>

#ifdef __cplusplus
extern "C" {
#endif

struct ferplayer {
	struct fentity base;
};

void _frPlayerInit(struct ferplayer* instance);
void _frPlayerDraw(struct ferplayer* instance);
void _frPlayerUpdate(struct ferplayer* instance);
void _frPlayerDamage(struct ferplayer* instance, float hp);

#ifdef __cplusplus
}
#endif
