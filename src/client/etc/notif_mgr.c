
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "fightable/string.h"
#include "raylib.h"
#include <fightable/notif_mgr.h>
#include <fightable/state.h>

#define POPUP_GAP 35
#define POPUP_TIMEOUT (float)5.f

RSB_ARRAY_IMPL_GEN(struct fnotif_mgr_entry, _fnotif_mgr_entry);

void _fNotifMgrInit() {
    if (__state.notifications) return;
    __state.notifications = RSBCreateArray_fnotif_mgr_entry();
}
void _fNotifMgrSend(const char *message) {
    if (!__state.notifications) _fNotifMgrInit();
    if (!message) message = "(null)";

    int used_offset = -1;

    for (int i = 0; i < (__state.overlay_framebuffer.texture.width / POPUP_GAP) + 1; i++) {
        int offset = POPUP_GAP * i;
        unsigned char has_offset = 0;

        for (int j = 0; j < __state.notifications->added_elements; j++) {
            struct fnotif_mgr_entry *e = __state.notifications->objects + j;
            if (e->offset == offset) {
                has_offset = 1;
                break;
            }
        }

        if (!has_offset) {
            used_offset = offset;
            break;
        }
    }

    if (used_offset < 0) {
        TraceLog(LOG_WARNING, "Popup %s cannot be placed, because there is no space left on the screen", message);
        return;
    }

    struct fnotif_mgr_entry e = {_fNotifObjectCreate(message), 0.f, 0, used_offset};

    e.popup->box.x = e.offset;
    _fNotifObjectApplyOpenAnimation(e.popup);

    RSBAddElement_fnotif_mgr_entry(__state.notifications, e);
}
void _fNotifMgrUpdate() {
    if (!__state.notifications) _fNotifMgrInit();

    if (IsKeyPressed(KEY_T)) {
        _fNotifMgrSend("Hello, World!\nMultiline text is\nsupported too");
    }

    float delta = GetFrameTime();

    for (int i = 0; i < __state.notifications->added_elements; i++) {
        struct fnotif_mgr_entry *e = __state.notifications->objects + i;
        if (!e->closing) {
            e->time += delta;

            if (e->time > POPUP_TIMEOUT) {
                TraceLog(LOG_INFO, "Closing notification %s", e->popup->copied_message);
                _fNotifObjectApplyCloseAnimation(e->popup);
                e->closing = 1;
            }
        } else {
            if (e->popup->closed) {
                _fNotifObjectDestroy(e->popup, 1);
                RSBPopElementAtIndex_fnotif_mgr_entry(__state.notifications, i);
                break;
            }
        }
    }

    for (int i = 0; i < __state.notifications->added_elements; i++) {
        struct fnotif_mgr_entry *e = __state.notifications->objects + i;
        _fNotifObjectDraw(e->popup);
    }
}
