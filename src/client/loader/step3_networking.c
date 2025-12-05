
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/mp_shared.h>
#include <fraylib.h>

void _fLoaderMainPrepareNetworking() {
#ifndef _DISABLE_MP_SERVER_
    TraceLog(LOG_INFO, "[LOADER] Preparing multiplayer features");
    _fMpInit();
#else
    TraceLog(LOG_INFO, "[LOADER] Multiplayer functionality is disabled; not preparing networking");
#endif
}
