
#include <fightable/state.h>
#include <fightable/renderer.h>
#include <fightable/storage.h>
#include <fightable/notif_mgr.h>
#include <fightable/level.h>
#include <stdio.h>

unsigned char _fLoaderClose() {
    _fTilemapUnload(__state.tilemap);
    _fTilemapUnload(__state.tilemap2);
    UnloadRenderTexture(__state.framebuffer);
    UnloadRenderTexture(__state.overlay_framebuffer);

    if (__state.current_level) {
        UnloadTexture(__state.current_level->background_tile);
        free(__state.current_level->objects);
    }

    __state.sound_engine.should_stop = 1;
    pthread_join(__state.sound_thread, NULL);

    _fConfigSave(&__state.config);

    return 1;
}
