
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "fightable/intvec.h"
#include "fightable/renderer.h"
#include <fightable/a_clippy.h>
#include <fightable/tilemap.h>
#include <fightable/state.h>
#include <fightable/distance.h>

struct fa_clippy _fAssistantClippyCreate(IVector2 base_pos) {
    struct fa_clippy obj = {};
    obj.pos = base_pos;

    return obj;
}
void _fAssistantClippyUpdate(struct fa_clippy *obj) {
    if (!obj) return;

    obj->time += (double)GetFrameTime();
}
void _fAssistantClippyDraw(struct fa_clippy *obj) {
    if (!obj) return;

    double s = sin(obj->time) * ((double)__state.tilemap->tile_size.y * 0.4);
    IVector2 new_pos = obj->pos;

    new_pos.y += (int)ceil(s);

    unsigned char is_flipped = new_pos.x > (__state.framebuffer.texture.width / 2);

    IVector2 sprite_center = new_pos;
    sprite_center.x += __state.tilemap->tile_size.x / 2;
    sprite_center.y += __state.tilemap->tile_size.y;

    Color c1 = WHITE;
    c1.a = (unsigned char)fmax(32.f, 255.f * _fDistPointToCircle(_fGetMousePosPix(), _fImathToVFloat(sprite_center), __state.tilemap->tile_size.x * 8));

    _fTilemapDrawMegatile(__state.tilemap, new_pos, (IVector2){18, 6}, (IVector2){1, 2}, is_flipped, 0, c1);
    _fTilemapDraw(__state.tilemap, new_pos, (IVector2){19, 6}, is_flipped, 0, c1);
}
