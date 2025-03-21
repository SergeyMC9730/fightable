
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/button.h>
#include <fightable/state.h>
#include <fightable/text.h>
#include <math.h>
#include <fightable/renderer.h>
#include <stdio.h>

unsigned char _fSquareButtonDraw(struct fsquare_button *btn) {
    Vector2 mpos = _fGetMousePosPix();

    RLRectangle r = {
        .x = btn->position.x,
        .y = btn->position.y,
        .width = 0,
        .height = 0
    };

    IVector2 cur_pos = btn->position;
    IVector2 btn_tile_offset = {29, 6};
    IVector2 btn_label_offset = {0};

    // _fTilemapDraw(*__state.tilemap, cur_pos, (IVector2){35 + btn_tile_offset.x, 1 + btn_tile_offset.y}, 0, 0, WHITE);

    cur_pos.x += __state.tilemap->tile_size.x;

    cur_pos.x += __state.tilemap->tile_size.x;

    int size = 10;
    int center_x = (size - 8) / 2;

    RLRectangle btn_rect = {btn->position.x + 3, btn->position.y + 3, 10, 10};
    unsigned char ret = 0;

    if (CheckCollisionPointRec(mpos, btn_rect) ){
        btn_tile_offset.x = 31;

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            btn_tile_offset.x = 33;
            btn_label_offset.y--;
        }
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            btn->flag = !btn->flag;
            ret = 1;
            if (btn->on_click) {
                btn->on_click(btn);
            }
        }
    }

    cur_pos = btn->position;

    _fTilemapDrawMegatile(__state.tilemap, cur_pos, btn_tile_offset, (IVector2){2, 2}, 0, 0, WHITE);

    switch (btn->type) {
        case SQRBTN_CHECKBOX: {
            if (!btn->flag) break;

            _fTilemapDraw(__state.tilemap, (IVector2){btn->position.x + center_x + 3, btn->position.y + 3 + btn_label_offset.y}, (IVector2){27, 6}, 0, 0, WHITE);

            break;
        }
        case SQRBTN_EXIT: {
            _fTilemapDraw(__state.tilemap, (IVector2){btn->position.x + center_x + 3, btn->position.y + 3 + btn_label_offset.y}, (IVector2){28, 6}, 0, 0, WHITE);
            break;
        }
        case SQRBTN_REVCHECKBOX: {
            if (btn->flag) break;

            _fTilemapDraw(__state.tilemap, (IVector2){btn->position.x + center_x + 3, btn->position.y + 3 + btn_label_offset.y}, (IVector2){27, 6}, 0, 0, WHITE);

            break;
        }
    }

    cur_pos.x += __state.tilemap->tile_size.x;

    return ret;
}

unsigned char _fSquareButtonDrawSimple(IVector2 pos, Color tint, unsigned char type, unsigned char flag) {
    struct fsquare_button btn = {0};
    btn.position = pos;
    btn.tint = tint;
    btn.type = type;
    btn.flag = flag;
    return _fSquareButtonDraw(&btn);
}
