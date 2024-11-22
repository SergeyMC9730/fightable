#include <fightable/state.h>
#include <fightable/intro.h>

#ifdef TARGET_ANDROID
#include <android_native_app_glue.h>
#endif

void _fIntroMenuDraw() {
    _fIntroMenuDrawBackground();

    if (__state.menu_perform_move) {
        float delta = GetFrameTime();

        __state.time1 += delta;
        __state.menu_cur_x = -__state.framebuffer.texture.width * __state.time1;

        if (__state.time1 >= 1.f) {
            __state.time1 = 1.f;
        }
    }

    switch (__state.menu_state) {
        case INTRO_MENU_BASE_SELECTOR: {
            _fIntroMenuProcessBase();

            break;
        }
        case INTRO_MENU_PLAY: {
            _fIntroMenuProcessPlay();

            break;
        }
        case INTRO_MENU_OPTIONS: {
            _fIntroMenuProcessOptions();

            break;
        }

        default: {}
    }
}