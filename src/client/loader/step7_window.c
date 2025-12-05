
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/loader.h>
#include <fraylib.h>

void _fLoaderMainPrepareWindow() {
    TraceLog(LOG_INFO, "[LOADER] Setting up window flags");

    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetExitKey(KEY_NULL);
    SetWindowIcon(RlLoadImage("assets/textures/icon.png"));
    SetTargetFPS(_fLoaderMainPrepareValidFps());
}
