#include <fraylib.h>
#include <time.h>

void _fLoaderMainPrepareRng() {
    TraceLog(LOG_INFO, "[LOADER] Generating PRNG's seed");
    SetRandomSeed(time(0));
}
