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
