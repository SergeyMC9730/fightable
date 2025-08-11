
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/renderer.h>
#include <nt5emul/renderer_event.h>
#include <fightable/state.h>
#include <variant>

using fschel_object = std::variant<foverlay_callback, renderer_event_t>;

struct fdraw_scheduler {
    std::vector<fschel_object> callbacks_a;
    std::vector<fschel_object> callbacks_b;
    bool inside_scheduler;

    void attach(const renderer_event_t &func) {
        if (!func.callback) return;

        if (inside_scheduler) {
            callbacks_b.push_back(func);
            TraceLog(LOG_INFO, "%d: scheduling inside scheduler", __LINE__);
        } else {
            callbacks_a.push_back(func);
        }
    }
    void attach(const foverlay_callback &func) {
        if (!func) return;

        if (inside_scheduler) {
            callbacks_b.push_back(func);
            TraceLog(LOG_INFO, "%d: scheduling inside scheduler", __LINE__);
        } else {
            callbacks_a.push_back(func);
        }
    }

    void iterate() {
        Vector2 mouse_pos = _fGetMousePosOverlay();

        inside_scheduler = true;
        callbacks_b.clear();

        for (const auto &callback : callbacks_a) {
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

        inside_scheduler = false;
        callbacks_a = callbacks_b;
    }
};

static struct fdraw_scheduler __overlay_schel;
static struct fdraw_scheduler __post_draw_schel;

void _fScheduleOverlayFunc(renderer_event_t func) {
    __overlay_schel.attach(func);
}
void _fScheduleOverlayFunc(const foverlay_callback &func) {
    __overlay_schel.attach(func);
}

void _fSchedulerIterateOverlays() {
    __overlay_schel.iterate();
}

unsigned char _fSchedulerInOverlay() {
    return (unsigned char)__overlay_schel.inside_scheduler;
}

void _fSchedulePostDrawFunc(renderer_event_t on_draw) {
    __post_draw_schel.attach(on_draw);
}
void _fSchedulerIteratePostDraws() {
    __post_draw_schel.iterate();
}
unsigned char _fSchedulerInPostDraw() {
    return (unsigned char)__post_draw_schel.inside_scheduler;
}

unsigned char _fSchedulerInScheduler() {
    return _fSchedulerInOverlay() || _fSchedulerInPostDraw();
}
