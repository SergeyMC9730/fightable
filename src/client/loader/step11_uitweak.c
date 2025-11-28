#include <fraylib.h>
#include <fightable/state.h>
#include <fightable/renderer.h>
#include <args.h>
#include <nt5emul/tui/environment.h>
#include <fightable/level.h>
#include <fightable/editor.h>

void _fLoaderMainTweakUiSettings(Vector2 *actual_sz, Vector2 *editor_sz) {
    TraceLog(LOG_INFO, "[LOADER] Tweaking UI settings");

    __state.base_game_size = *actual_sz;
    __state.editor_size = *editor_sz;
}
