
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "fightable/intvec.h"
#include <fightable/button.h>
#include <fightable/state.h>
#include <fightable/text.h>
#include <math.h>
#include <fightable/renderer.h>
#include <fightable/translation.h>

unsigned char _fButtonDraw(struct fbutton *btn) {
    if (!btn || !btn->text) return 0;

    Vector2 mpos = _fGetMousePosPix();
    IVector2 text_sz = _fTextMeasure(&__state.text_manager, btn->text);

    if (text_sz.x == 0) return 0;

    int msz = _fButtonMeasureSizeSimple(btn->text);
    int tiles = msz / __state.tilemap->tile_size.x;

    IVector2 cur_pos = btn->position;
    IVector2 btn_tile_offset = {0};
    IVector2 btn_label_offset = {0};

    int size = msz;
    int center_x = (size - text_sz.x) / 2;

    RLRectangle btn_rect = {btn->position.x, btn->position.y + 1, size, 7};

    unsigned char ret = 0;

    if (CheckCollisionPointRec(mpos, btn_rect) ){
        btn_tile_offset.y++;

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            btn_tile_offset.y++;
            btn_label_offset.y--;
        }
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            ret = 1;
            if (btn->on_click) {
                btn->on_click(btn);
            }
        }
    }

    cur_pos = btn->position;

    _fTilemapDraw(__state.tilemap, cur_pos, (IVector2){35 + btn_tile_offset.x, 1 + btn_tile_offset.y}, 0, 0, btn->tint);

    if (tiles == 0) cur_pos.x += __state.tilemap->tile_size.x;

    for (int i = 0; i < tiles; i++) {
        if (i == 0) {
            _fTilemapDraw(__state.tilemap, cur_pos, (IVector2){35 + btn_tile_offset.x, 1 + btn_tile_offset.y}, 0, 0, btn->tint);
        } else if (i == tiles - 1) {
            _fTilemapDraw(__state.tilemap, cur_pos, (IVector2){37 + btn_tile_offset.x, 1 + btn_tile_offset.y}, 0, 0, btn->tint);
        } else {
            _fTilemapDraw(__state.tilemap, cur_pos, (IVector2){36 + btn_tile_offset.x, 1 + btn_tile_offset.y}, 0, 0, btn->tint);
        }

        cur_pos.x += __state.tilemap->tile_size.x;
    }

    if (btn->tr_alt_id && _fTranslationGetCurrentLanguage() != FLI_EnUs) {
        const char *alt = _fTranslationGetString(btn->tr_alt_id);
        Vector2 text_sz_o = _fPosFramebufferToOverlay(_fImathToVFloat(text_sz));
        Vector2 def_text_sz = MeasureTextEx(__state.ubuntu_mono64, alt, 64, 0.5);
        Vector2 scaling = {text_sz_o.x / def_text_sz.x, text_sz_o.y / def_text_sz.y};
        float m = (scaling.x > scaling.y) ? scaling.y : scaling.x;
        m *= 1.2f;

        def_text_sz.x *= m;
        def_text_sz.y *= m;

        Vector2 new_offset = _fPosFramebufferToOverlay(_fImathToVFloat(btn_label_offset));

        RLRectangle new_rect = _fRectFramebufferToOverlay(btn_rect);
        Vector2 new_center = (Vector2){new_rect.x + ((new_rect.width - def_text_sz.x) / 2) + new_offset.x, new_rect.y + ((new_rect.height - def_text_sz.y) / 2) + new_offset.y};

        _fTranslationQueueTextDraw(_fVFloatToI(new_center), btn->tr_alt_id, m, btn->tint, 0);
    } else {
        _fTextDraw(&__state.text_manager, btn->text, (IVector2){btn->position.x + center_x + btn_label_offset.x, btn->position.y + 2 + btn_label_offset.y}, btn->tint, 0);
    }

    return ret;
    // DrawRectangleLinesEx(btn_rect, 1.f, RED);
}

unsigned char _fButtonDrawSimple(const char *text, IVector2 pos, Color tint, const char *alt_id) {
    struct fbutton btn = {0};
    btn.text = text;
    btn.position = pos;
    btn.tint = tint;
    btn.tr_alt_id = alt_id;

    return _fButtonDraw(&btn);
}

int _fButtonMeasureSize(struct fbutton *btn) {
    if (!btn || !btn->text) return 0;

    IVector2 text_sz = _fTextMeasure(&__state.text_manager, btn->text);
    if (text_sz.x == 0) return 0;

    int tiles = (int)ceil((float)text_sz.x / (float)__state.tilemap->tile_size.x) + 1;

    return (tiles) * __state.tilemap->tile_size.x;
}
int _fButtonMeasureSizeSimple(const char *text) {
    struct fbutton btn = {0};
    btn.text = text;

    return _fButtonMeasureSize(&btn);
}
