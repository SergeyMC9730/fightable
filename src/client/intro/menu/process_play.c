#include <fightable/intro.h>
#include <fightable/state.h>
#include <fightable/button.h>
#include <fightable/renderer.h>

void _fIntroMenuProcessPlay() {
    int wx = __state.framebuffer.texture.width;
    int wy = __state.framebuffer.texture.height;

    int w = 80;

    RLRectangle area = (RLRectangle){(wx - w) / 2 + __state.menu_cur_x, 42, w, 50};
    Color tint = WHITE;

    unsigned char btn_flag = _fButtonDrawSimple("BACK", (IVector2) { (wx - (3 * __state.tilemap->tile_size.x)) / 2, area.y + area.height + 2 }, tint);

    if (btn_flag || IsKeyPressed(KEY_ESCAPE)) {
        __state.menu_state = INTRO_MENU_BASE_SELECTOR;
        UnloadTexture(__state.playbtn_container);

        return;
    }

    area.width = __state.playbtn_container.width;
    area.height = __state.playbtn_container.height;

    DrawTexture(__state.playbtn_container, area.x, area.y, tint);

    {
        RLRectangle r = area;
        Vector2 mpos = _fGetMousePosPix();

        r.height /= 2;
        if (CheckCollisionPointRec(mpos, r)) {
            BeginBlendMode(BLEND_MULTIPLIED);
            DrawRectangleRec(r, (Color){255, 255, 255, 64});
            EndBlendMode();

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                _fIntroMenuOnMpJoin();
            }
        }
        
        r.y += r.height;
        if (CheckCollisionPointRec(mpos, r)) {
            BeginBlendMode(BLEND_MULTIPLIED);
            DrawRectangleRec(r, (Color){255, 255, 255, 64});
            EndBlendMode();

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                _fIntroMenuOnMpCreate();
            }
        }
    }
}