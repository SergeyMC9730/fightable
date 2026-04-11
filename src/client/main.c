
//          Sergei Baigerov 2024 - 2026.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/tilemap.h>
#include <fightable/init.h>
#include <fightable/loader.h>
#include <fightable/state.h>
#include <stdio.h>

struct ftilemap __tilemap;
struct ftilemap __tilemap2;

#ifndef TARGET_ANDROID
unsigned int UI_SCALE = 4;
#else
unsigned int UI_SCALE = 5;
#endif

#ifdef TARGET_ANDROID
struct android_app *GetAndroidApp();
#endif

void _fInit(int argc, char **argv, struct fightable_init_data *data) {
#ifdef TARGET_ANDROID
    __state.system = GetAndroidApp();
#endif

    if (!IsWindowReady()) {
        printf("Fightable cannot operate without preinitialized window\n");
        // return;
    }

    Vector2 win_sz = {800, 600};
    Vector2 actual_sz = win_sz;
    Vector2 editor_sz = {255 / (5.f / (float)UI_SCALE), 0};

    _fLoaderMainPrepareLogging();
    _fLoaderMainPrepareStorage();
    _fLoaderMainPrepareNetworking();
    _fLoaderMainPrepareAudio();
    _fLoaderMainPrepareWindow();
    Vector2 ui_scaling = _fLoaderMainPrepareUi(&win_sz, &actual_sz, &editor_sz);
    _fLoaderMainProcessAssets(&__tilemap, &__tilemap2);
    _fLoaderMainProcessArguments(&win_sz, &actual_sz, &editor_sz, argc, argv, &ui_scaling);
    _fLoaderMainTweakUiSettings(&actual_sz, &editor_sz);
    _fLoaderMainPrepareEnvironment(&actual_sz, &win_sz, &ui_scaling);

    data->close = _fLoaderClose;
    data->draw = _fLoaderDraw;
}
