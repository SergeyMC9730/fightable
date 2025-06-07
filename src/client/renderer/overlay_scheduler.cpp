
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/renderer.h>
#include <nt5emul/renderer_event.h>
#include <fightable/state.h>
#include <variant>

static std::vector<std::variant<foverlay_callback, renderer_event_t>>   __overlay_callbacks = {};
static std::vector<std::variant<foverlay_callback, renderer_event_t>>   __overlay_callbacksB = {};
static bool                                                             __overlay_inScheduler = false;

void _fScheduleOverlayFunc(renderer_event_t func) {
    if (!func.callback) return;

    if (__overlay_inScheduler) {
        __overlay_callbacksB.push_back(func);
        TraceLog(LOG_INFO, "%d: scheduling inside scheduler", __LINE__);
    } else {
        __overlay_callbacks.push_back(func);
    }
}
void _fScheduleOverlayFunc(const foverlay_callback &callback) {
    if (!callback) return;

    if (__overlay_inScheduler) {
        __overlay_callbacksB.push_back(callback);
        TraceLog(LOG_INFO, "%d: scheduling inside scheduler", __LINE__);
    } else {
        __overlay_callbacks.push_back(callback);
    }
}

void _fSchedulerIterateOverlays() {
    Vector2 mouse_pos = _fGetMousePosOverlay();

    __overlay_inScheduler = true;
    __overlay_callbacksB.clear();

    for (const auto &callback : __overlay_callbacks) {
        if (std::holds_alternative<foverlay_callback>(callback)) {
            const auto &c = std::get<foverlay_callback>(callback);

            if (c) {
                c(mouse_pos);
            }
        } else if (std::holds_alternative<renderer_event_t>(callback)) {
            const auto &c = std::get<renderer_event_t>(callback);

            if (c.callback) {
                c.callback(c.user);
            }
        }

        // EndMode2DStacked();
    }

    __overlay_inScheduler = false;

    __overlay_callbacks.clear();
    __overlay_callbacks = __overlay_callbacksB;
}
