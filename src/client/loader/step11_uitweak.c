
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fraylib.h>
#include <fightable/state.h>

void _fLoaderMainTweakUiSettings(Vector2 *actual_sz, Vector2 *editor_sz) {
    TraceLog(LOG_INFO, "[LOADER] Tweaking UI settings");

    __state.base_game_size = *actual_sz;
    __state.editor_size = *editor_sz;
}
