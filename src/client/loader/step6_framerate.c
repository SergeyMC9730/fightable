
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fraylib.h>
#include <fightable/sort.h>

int _fLoaderMainPrepareValidFps() {
    TraceLog(LOG_INFO, "[LOADER] Trying to figure out valid framerate");
#define MAX_MONITORS 8
    int monitor_refresh_rates[MAX_MONITORS] = {};
    for (unsigned int i = 0; i < GetMonitorCount(); i++) {
        monitor_refresh_rates[i] = GetMonitorRefreshRate(i);
    }
    _fSortIntDescending(monitor_refresh_rates, MAX_MONITORS);
    int highest_refresh_rate = monitor_refresh_rates[0];
    if (highest_refresh_rate == 0) {
        TraceLog(LOG_INFO, "[LOADER] Cannot get highest refresh rate");
        highest_refresh_rate = 60;
    }
    TraceLog(LOG_INFO, "[LOADER] Highest refresh rate: %d", highest_refresh_rate);
    return highest_refresh_rate;
}
