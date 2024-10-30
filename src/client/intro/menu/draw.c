#include <fightable/state.h>
#include <fightable/intro.h>

#ifdef TARGET_ANDROID
#include <android_native_app_glue.h>
#endif

void _fIntroMenuDraw() {
    _fIntroMenuDrawBackground();

    switch (__state.menu_state) {
        case INTRO_MENU_BASE_SELECTOR: {
            _fIntroMenuProcessBase();

            break;
        }

        case INTRO_MENU_PLAY: {
            _fIntroMenuProcessPlay();

            break;
        }

        default: {}
    }
}