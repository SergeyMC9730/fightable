
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/rplayer.h>
#include <fightable/tilemap.h>
#include <fightable/level.h>

void _frPlayerDraw(struct ferplayer *instance) {
	if (instance->base.dead) return;

	IVector2 pos = _fEntityGetDrawingPos(&instance->base);

	_fTilemapDraw(instance->base.level->tilemap, pos, (IVector2) { 12, 0 }, (unsigned char)instance->base.render_direction, 0, instance->base.tint);
	_fEntityDrawAccessory((struct fentity *)instance);
}
