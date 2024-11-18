#include <fightable/button.h>
#include <raylib.h>
#include <fightable/state.h>
#include <fightable/text.h>
#include <math.h>
#include <fightable/renderer.h>

unsigned char _fSquareButtonDraw(struct fbutton *btn) {
    if (!btn || !btn->text) return 0;

    Vector2 mpos = _fGetMousePosPix();
    IVector2 text_sz = _fTextMeasure(&__state.text_manager, btn->text);

    if (text_sz.x == 0) return 0;

    Rectangle r = {
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
    int center_x = (size - text_sz.x) / 2;

    Rectangle btn_rect = {btn->position.x + 3, btn->position.y + 3, 10, 10};
    DrawRectangle(btn_rect.x, btn_rect.y, btn_rect.width, btn_rect.height, MAROON);
    unsigned char ret = 0;

    if (CheckCollisionPointRec(mpos, btn_rect) ){ 
        btn_tile_offset.x = 31;

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            btn_tile_offset.x = 33;    
            btn_label_offset.y--;
        }
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            ret = 1;
        }
    }

    cur_pos = btn->position;

    _fTilemapDrawMegatile(__state.tilemap, cur_pos, btn_tile_offset, (IVector2){2, 2}, 0, 0, WHITE);

    cur_pos.x += __state.tilemap->tile_size.x;

    _fTextDraw(&__state.text_manager, btn->text, (IVector2){btn->position.x + center_x + btn_label_offset.x, btn->position.y + btn_label_offset.y}, btn->tint, 0);

    return ret;
    // DrawRectangleLinesEx(btn_rect, 1.f, RED);
}

unsigned char _fSquareButtonDrawSimple(const char *text, IVector2 pos, Color tint) {
    struct fbutton btn = {0};
    btn.text = text;
    btn.position = pos;
    btn.tint = tint;
    
    return _fSquareButtonDraw(&btn);
}