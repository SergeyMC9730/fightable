
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fraylib.h>
#include <fightable/state.h>
#include <fightable/renderer.h>
#include <fightable/intro.h>
#include <fightable/notif_mgr.h>
#include <fightable/translation.h>

void _fLoaderMainPrepareEnvironment(Vector2 *actual_sz, Vector2 *win_sz, Vector2 *ui_scaling) {
    TraceLog(LOG_INFO, "[LOADER] Preparing game environment");

#ifndef DEBUG
    _fIntroInit();
#endif

    RenderTexture2D txt = LoadRenderTexture(win_sz->x / UI_SCALE, win_sz->y / UI_SCALE);
    __state.framebuffer = txt;

    __state.overlay_framebuffer = LoadRenderTexture(actual_sz->x / ui_scaling->x, actual_sz->y / ui_scaling->y);

    unsigned char shake_lock[8] = {0};

    _fConfigInit(&__state.config);

    if (__state.song_id == -1) {
        TraceLog(LOG_ERROR, "Could not initialize intro properly");
        _fNotifMgrSend(_fTranslationGetString("notification.intro.load_failed"));
        _fIntroMenuInit();
    }

    __state.gfx.fade_v.should_process = 1;

    _fKeyboardRegister(&__state.kbd, KEY_ESCAPE);
    _fKeyboardRegister(&__state.kbd, KEY_F1);
    _fKeyboardRegister(&__state.kbd, KEY_A);
    _fKeyboardRegister(&__state.kbd, KEY_D);
    _fKeyboardRegister(&__state.kbd, KEY_LEFT);
    _fKeyboardRegister(&__state.kbd, KEY_RIGHT);
    _fKeyboardRegister(&__state.kbd, KEY_SPACE);

    SetTextLineSpacing((int)(15.f / GetWindowScaleDPI().y * 1.5f));

    __state.clippy = _fAssistantClippyCreate((IVector2){16, 16});
}
