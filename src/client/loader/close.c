
//          Sergei Baigerov 2024 - 2026.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/state.h>
#include <fightable/level.h>

unsigned char _fLoaderClose() {
    _fTilemapUnload(__state.tilemap);
    _fTilemapUnload(__state.tilemap2);
    UnloadRenderTexture(__state.framebuffer);
    UnloadRenderTexture(__state.overlay_framebuffer);
    UnloadFont(__state.unifont16);
    UnloadFont(__state.ubuntu_mono64);

    if (__state.current_level) {
        UnloadTexture(__state.current_level->background_tile);
        free(__state.current_level->objects);
    }

    __state.sound_engine.should_stop = 1;
    pthread_join(__state.sound_thread, NULL);

    _fConfigSave(&__state.config);

    return 1;
}
