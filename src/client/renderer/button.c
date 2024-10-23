#include <fightable/button.h>
#include <raylib.h>
#include <fightable/state.h>
#include <fightable/text.h>
#include <math.h>

unsigned char _fButtonDraw(struct fbutton *btn) {
    if (!btn || !btn->text) return 0;

    Vector2 mpos = GetMousePosition();
    mpos.x /= 5;
    mpos.y /= 5;

    IVector2 text_sz = _fTextMeasure(&__state.text_manager, btn->text);

    if (text_sz.x == 0) return 0;

    int additional_size = 0;

    if (text_sz.x > 22) {
        additional_size = (int)ceil(((float)text_sz.x / 22.f) - 1.f);
    }

    Rectangle r = {
        .x = btn->position.x,
        .y = btn->position.y,
        .width = 0,
        .height = 0
    };

    IVector2 cur_pos = btn->position;
    IVector2 btn_tile_offset = {};
    IVector2 btn_label_offset = {};

    // _fTilemapDraw(*__state.tilemap, cur_pos, (IVector2){35 + btn_tile_offset.x, 1 + btn_tile_offset.y}, 0, 0, WHITE);

    cur_pos.x += __state.tilemap->tile_size.x;

    for (int i = 0; i < 1 + additional_size; i++) {
        cur_pos.x += __state.tilemap->tile_size.x;
    }

    cur_pos.x += __state.tilemap->tile_size.x;

    int size = cur_pos.x - btn->position.x;
    int center_x = (size - text_sz.x) / 2;

    Rectangle btn_rect = {btn->position.x, btn->position.y + 1, size, 7};

    unsigned char ret = 0;

    if (CheckCollisionPointRec(mpos, btn_rect) ){ 
        btn_tile_offset.y++;

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            btn_tile_offset.y++;
            btn_label_offset.y--;
        }
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            ret = 1;
        }
    }

    cur_pos = btn->position;

    _fTilemapDraw(*__state.tilemap, cur_pos, (IVector2){35 + btn_tile_offset.x, 1 + btn_tile_offset.y}, 0, 0, WHITE);

    cur_pos.x += __state.tilemap->tile_size.x;

    for (int i = 0; i < 1 + additional_size; i++) {
        _fTilemapDraw(*__state.tilemap, cur_pos, (IVector2){36 + btn_tile_offset.x, 1 + btn_tile_offset.y}, 0, 0, WHITE);

        cur_pos.x += __state.tilemap->tile_size.x;
    }
    
    _fTilemapDraw(*__state.tilemap, cur_pos, (IVector2){37 + btn_tile_offset.x, 1 + btn_tile_offset.y}, 0, 0, WHITE);

    _fTextDraw(&__state.text_manager, btn->text, (IVector2){btn->position.x + center_x + btn_label_offset.x, btn->position.y + 2 + btn_label_offset.y}, WHITE, 0);

    return ret;
    // DrawRectangleLinesEx(btn_rect, 1.f, RED);
}