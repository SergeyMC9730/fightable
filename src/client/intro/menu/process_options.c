#include <fightable/intro.h>
#include <fightable/state.h>
#include <fightable/button.h>
#include <fightable/flags.h>
#include <fightable/slider.h>
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

    unsigned char btn_flag = _fButtonDrawSimple("BACK", (IVector2) { (wxx - (3 * __state.tilemap->tile_size.x)) / 2, 94 }, tint);

    if (btn_flag || IsKeyPressed(KEY_ESCAPE)) {
        __state.menu_state = INTRO_MENU_BASE_SELECTOR;
        UnloadTexture(__state.playbtn_container);

        return;
    }

    if(_fSquareButtonDraw(&__state.config.vsync_btn)) {
        if (IsWindowState(FLAG_VSYNC_HINT)) ClearWindowState(FLAG_VSYNC_HINT);
        else SetWindowState(FLAG_VSYNC_HINT);    
    }
    
    DrawTexture(__state.playbtn_container, area.x, area.y, tint);

    _fSliderDraw(&__state.config.volume_slider);
    __state.sound_engine.volume = __state.config.volume_slider.progress;

    TraceLog(LOG_INFO, "percentage: %f", __state.config.volume_slider.progress * 100.f);
}