
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "nt5emul/renderer_animation.h"
#include "nt5emul/renderer_keyframe.h"
#include <fightable/notif_mgr.h>
#include <fightable/state.h>

#define POPUP_GAP 2
#define POPUP_TIMEOUT (float)5.f

RSB_ARRAY_IMPL_GEN(struct fnotif_mgr_entry, _fnotif_mgr_entry);

void _fNotifMgrRealign() {
    double delta = (double)GetFrameTime();
    double x = 0;

    if (__state.notifications->len == 0) return;

    float *new_offsets = (float *)MemAlloc(sizeof(float) * __state.notifications->len);

    for (int i = 0; i < __state.notifications->len; i++) {
        struct fnotif_mgr_entry *e = __state.notifications->objects + i;
        if (e->popup->closed || e->closing) continue;

        new_offsets[i] = x;
        x += e->popup->box.width;
    }

    for (int i = 0; i < __state.notifications->len; i++) {
        struct fnotif_mgr_entry *e = __state.notifications->objects + i;

        memset(&e->move_anim, 0, sizeof(struct renderer_animation));
        e->move_anim.keyframes = &e->linked_keyframe;
        e->move_anim.count = 1;
        e->move_anim.early_value = e->popup->box.x;
        e->move_anim.current_value = e->popup->box.x;
        e->move_anim.starting_value = e->popup->box.x;
        e->move_anim.local_current_value = e->popup->box.x;
        e->move_anim.delta = delta;
        e->move_anim.anim_id = 0;
        e->move_anim.valid = 1;

        e->linked_keyframe.length = 0.3;
        e->linked_keyframe.ending_value = new_offsets[i] - e->popup->box.x;
        e->linked_keyframe.easing = TOOutExpo;

        if (e->offscreen && new_offsets[i] < __state.overlay_framebuffer.texture.width) {
            e->offscreen = 0;
        }
    }

    MemFree(new_offsets);
}

void _fNotifMgrInit() {
    if (__state.notifications) return;
    __state.notifications = RSBCreateArray_fnotif_mgr_entry();
}
void _fNotifMgrSendWithTime(const char *message, double tlength) {
    if (!__state.notifications) _fNotifMgrInit();
    if (!message) message = "(null)";

    int used_offset = -1;

    for (int i = 0; i < (0xFFFF / POPUP_GAP) + 1; i++) {
        int offset = POPUP_GAP * i;
        unsigned char has_offset = 0;

        for (int j = 0; j < __state.notifications->len; j++) {
            struct fnotif_mgr_entry *e = __state.notifications->objects + j;
            unsigned char chk = (e->popup->box.x + e->popup->box.width) > (float)offset;
            if (chk) {
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

    struct fnotif_mgr_entry e = {_fNotifObjectCreate(message), 0.f, tlength, 0, used_offset};

    e.popup->box.x = e.offset;
    e.max_time = tlength;
    e.offscreen = (e.popup->box.x + e.popup->box.width) > (float)__state.overlay_framebuffer.texture.width && (__state.overlay_framebuffer.texture.width * __state.overlay_framebuffer.texture.height != 0);
    _fNotifObjectApplyOpenAnimation(e.popup);

    RSBAddElement_fnotif_mgr_entry(__state.notifications, e);

    _fNotifMgrRealign();
}
void _fNotifMgrSend(const char *message) {
    _fNotifMgrSendWithTime(message, POPUP_TIMEOUT);
}
void _fNotifMgrUpdate() {
    if (!__state.notifications) _fNotifMgrInit();

    if (IsKeyPressed(KEY_T)) {
        _fNotifMgrSendWithTime("* human.. i remember you're\n  <cred,orange>genocides..\n ", POPUP_TIMEOUT);
    }

    float delta = GetFrameTime();

    for (int i = 0; i < __state.notifications->len; i++) {
        struct fnotif_mgr_entry *e = __state.notifications->objects + i;
        if (!e->closing) {
            if (!e->offscreen && e->max_time != 0) {
                e->time += delta;
                e->popup->complete_progress = e->time / e->max_time;
            }

            // TraceLog(LOG_INFO, "%f %f %d", (float)e->time, (float)e->max_time, e->offscreen);

            if (e->time >= e->max_time) {
                TraceLog(LOG_INFO, "Closing notification %s", _fMultilineTextInstanceGetText(e->popup->text));
                _fNotifObjectApplyCloseAnimation(e->popup);
                e->closing = 1;
            }
        } else {
            if (e->popup->closed) {
                _fNotifObjectDestroy(e->popup, 1);
                RSBPopElementAtIndex_fnotif_mgr_entry(__state.notifications, i);

                _fNotifMgrRealign();

                break;
            }
        }
    }

    for (int i = 0; i < __state.notifications->len; i++) {
        struct fnotif_mgr_entry *e = __state.notifications->objects + i;

        if (e->offscreen) continue;

        if (e->move_anim.valid && !e->move_anim.completed) {
            _ntRendererUpdateAnimation(&e->move_anim);
            e->popup->box.x = _ntRendererGetAnimationResult(&e->move_anim, 0);
        }

        _fNotifObjectDraw(e->popup);
    }
}
