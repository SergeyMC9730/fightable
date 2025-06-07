
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "nt5emul/tui/environment.h"
#include "raylib.h"
#include "rsb/rsb_array_cstr.h"
#include <fightable/renderer.h>
#include <fightable/state.h>

void _fFileSelectorCallback2(void *) {
    _fCloseFileSelector();
}
void _fFileSelectorCallback(struct nt_file_selector_menu *menu, const char *file_path) {
    if (file_path) {
        TraceLog(LOG_INFO, "User selected \"%s\"", file_path);
    } else {
        TraceLog(LOG_INFO, "User aborted file selection");
    }

    renderer_event_t ev;
    ev.callback = _fFileSelectorCallback2;
    ev.user = NULL;

    _fScheduleOverlayFunc(ev);
}

void _fOpenFileSelector(const char *path, void (*callback)(struct nt_file_selector_menu *menu, const char *file_path)) {
    _fCloseFileSelector();

    if (!path) return;
    if (!callback) callback = _fFileSelectorCallback;

    int c = (int)((float)__state.overlay_framebuffer.texture.height / 16.f / _ntGetTuiEnvironment()->scaling - 7.f);

    __state.current_search_menu = _ntLoadFileSelector(path, c);
    __state.current_search_menu->callback = callback;
    __state.current_search_menu->base.x = 4;
    __state.current_search_menu->base.y = 3;
}

void _fCloseFileSelector() {
    if (__state.current_search_menu) {
        _ntUnloadFileSelector(__state.current_search_menu);
        __state.current_search_menu = NULL;
    }
}
