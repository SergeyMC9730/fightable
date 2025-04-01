
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/renderer.h>
#include <fightable/state.h>

void _fFileSelectorCallback2(void *) {
    _fCloseFileSelector();
}
void _fFileSelectorCallback(struct nt_file_selector_menu *menu, const char *file_path) {
    renderer_event_t ev;
    ev.callback = _fFileSelectorCallback2;
    ev.user = NULL;

    _fScheduleOverlayFunc(ev);
}

void _fOpenFileSelector(const char *path, void (*callback)(struct nt_file_selector_menu *menu, const char *file_path)) {
    _fCloseFileSelector();

    if (!path) return;
    if (!callback) callback = _fFileSelectorCallback;

    __state.current_search_menu = _ntLoadFileSelector(path, 16);
    __state.current_search_menu->callback = callback;
}

void _fCloseFileSelector() {
    _ntUnloadFileSelector(__state.current_search_menu);
}
