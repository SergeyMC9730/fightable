#include <fightable/loader.h>
#include <fraylib.h>

void _fLoaderMainPrepareWindow() {
    TraceLog(LOG_INFO, "[LOADER] Setting up window flags");

    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetExitKey(KEY_NULL);
    SetWindowIcon(RlLoadImage("assets/textures/icon.png"));
    SetTargetFPS(_fLoaderMainPrepareValidFps());
}
