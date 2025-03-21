
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <nt5emul/renderer_event.h>

enum ffade_type {
    FadeIn,
    FadeOut,
    FadeInOut
};

#include <fraylib.h>

struct gfx_manager {
    struct shake {
        double time;
        double max;
        double s;

        double x;
        double y;
    } shake_v;

    struct fade {
        double time;
        double max;

        renderer_event_t on_fade_out;

        Color begin;
        Color end;
        Color current;

        unsigned char should_process : 1;
        unsigned char do_not_display : 1;
        unsigned char callback_lock : 1;
    } fade_v;
};

void _fGfxShake(struct gfx_manager *m, float strength);
void _fGfxShakeConst(struct gfx_manager *m, float strength);

void _fGfxFadeIn(struct gfx_manager *m, Color end, double time);
void _fGfxFadeOut(struct gfx_manager *m, Color begin, double time);
void _fGfxFadeInOut(struct gfx_manager *m, Color begin, Color end, double time);

void _fGfxSetFadeOutFunc(struct gfx_manager *m, void (*callback)(void *user), void *user);

void _fGfxActivateDamageOverlay();
void _fGfxStopDamageOverlay();
void _fGfxDrawDamageOverlay();

void _fGfxUpdate(struct gfx_manager *m);
void _fGfxDraw(struct gfx_manager *m);

#ifdef __cplusplus
}
#endif
