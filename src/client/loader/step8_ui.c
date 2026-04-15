
//          Sergei Baigerov 2024 - 2026.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fraylib.h>
#include <fightable/state.h>
#include <fightable/renderer.h>

Vector2 _fLoaderMainPrepareUi(Vector2 *win_sz, Vector2 *actual_sz, Vector2 *editor_sz) {
    TraceLog(LOG_INFO, "[LOADER] Fixing window size properties");

    __state.window_scale = UI_SCALE;
    Vector2 ui_scaling = GetWindowScaleDPI();

    actual_sz->x *= ui_scaling.x;
    actual_sz->y *= ui_scaling.y;

    editor_sz->x *= ui_scaling.x;
    editor_sz->y *= ui_scaling.y;

#ifdef TARGET_ANDROID
    __state.initial_game_size = *win_sz;
    *actual_sz = (Vector2){0, 0};
    __state.ui_mode = PU_MOBILE;
#else
    __state.initial_game_size = *actual_sz;
    SetWindowSize(actual_sz->x, actual_sz->y);
    __state.ui_mode = PU_PC;
#endif

    return ui_scaling;
}
