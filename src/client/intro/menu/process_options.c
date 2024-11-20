#include <fightable/intro.h>
#include <fightable/state.h>
#include <fightable/button.h>
#include <fightable/flags.h>
#include <stdlib.h>
void _fIntroMenuProcessOptions() {
    int wx = 130;
    int wy = 52;

    int wxx = __state.framebuffer.texture.width;
    int wyy = __state.framebuffer.texture.height;

    int w = 80;

    IVector2 btncheck_pos = {10, 52};

    Color tint = WHITE;


    Rectangle area = (Rectangle){btncheck_pos.x + 10 + 2, btncheck_pos.y + 1, w, 10};
    bool checkmark = 1;
    unsigned char btn_flag = _fButtonDrawSimple("BACK", (IVector2) { (wxx - (3 * __state.tilemap->tile_size.x)) / 2, 94 }, tint);
    struct fcheckbox btn = {0};
    btn.position = (IVector2){10, area.y + 5};
    btn.tint = tint;
    btn.checkmark = v_sync_flag;
    __state.vsync_btn = btn;
    if (btn_flag || IsKeyPressed(KEY_ESCAPE)) {
        __state.menu_state = INTRO_MENU_BASE_SELECTOR;
        UnloadTexture(__state.playbtn_container);

        return;
    }
    if(_fSquareButtonDraw(&__state.vsync_btn)) {
            if (IsWindowState(FLAG_VSYNC_HINT)) ClearWindowState(FLAG_VSYNC_HINT);
            else SetWindowState(FLAG_VSYNC_HINT);    
    }
    DrawTexture(__state.playbtn_container, area.x, area.y, tint);
}