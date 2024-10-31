#include <fightable/gfx.h>

void _fGfxFadeIn(struct gfx_manager *m, Color end, double time) {
    _fGfxFadeInOut(m, BLANK, end, time);
}
void _fGfxFadeOut(struct gfx_manager *m, Color begin, double time) {
    _fGfxFadeInOut(m, begin, BLANK, time);
}

void _fGfxFadeInOut(struct gfx_manager *m, Color begin, Color end, double time) {
    if (time <= 0) return;

    m->fade_v.begin = begin;
    m->fade_v.end = end;
    m->fade_v.max = time;
    m->fade_v.time = 0;
    m->fade_v.should_process = 1;
    m->fade_v.callback_lock = 0;

    _fGfxSetFadeOutFunc(m, 0, 0);
}

void _fGfxSetFadeOutFunc(struct gfx_manager *m, void (*callback)(void *user), void *user) {
    m->fade_v.on_fade_out.callback = callback;
    m->fade_v.on_fade_out.user = user;
}

#include <fightable/color.h>

double _fGfxFadeCalculateValueMix(double a, double b, double equality) {
    return a + ((b - a) * equality);
}
Color _fGfxFadeCalculateMix(Color a, Color b, double equality) {
    // prevent nan and equality overflow situations
    if (equality >= 1.f) return b;
    if (equality <= 0.f) return a;

    Color newc;

    newc.r = (unsigned char)_fGfxFadeCalculateValueMix((double)a.r, (double)b.r, equality);
    newc.g = (unsigned char)_fGfxFadeCalculateValueMix((double)a.g, (double)b.g, equality);
    newc.b = (unsigned char)_fGfxFadeCalculateValueMix((double)a.b, (double)b.b, equality);
    newc.a = (unsigned char)_fGfxFadeCalculateValueMix((double)a.a, (double)b.a, equality);

    return newc;
}

#define STAGE_FADE_IN  0
#define STAGE_FADE_OUT 1

#include <stdio.h>

void _fGfxFadeUpdate(struct gfx_manager *m) {
    if (!m->fade_v.should_process) return;

    double delta = GetFrameTime();
    double half = m->fade_v.max / 2;

    m->fade_v.time += delta;
    if (m->fade_v.time > m->fade_v.max) {
        m->fade_v.time = m->fade_v.max;
    }

    double v = m->fade_v.time / half;
    double t = m->fade_v.time / m->fade_v.max;

    unsigned char stage = STAGE_FADE_IN;
    if (t >= 0.5) {
        stage = STAGE_FADE_OUT;
        if (!m->fade_v.callback_lock) {
            if (m->fade_v.on_fade_out.callback) {
                m->fade_v.on_fade_out.callback(m->fade_v.on_fade_out.user);
            }

            m->fade_v.callback_lock = 1;
        }
    }

    switch (stage) {
        case STAGE_FADE_IN: {
            m->fade_v.current = _fGfxFadeCalculateMix(BLANK, m->fade_v.begin, v);
            break;
        }
        case STAGE_FADE_OUT: {
            m->fade_v.current = _fGfxFadeCalculateMix(m->fade_v.begin, m->fade_v.end, v - 1.f);
            break;
        }
    }

    // printf("%f %f | %f %f | %d | %d,%d,%d,%d\n",
    //     m->fade_v.time, m->fade_v.max,
    //     v, t, stage,
    //     m->fade_v.current.r, m->fade_v.current.g,
    //     m->fade_v.current.b, m->fade_v.current.a
    // );
}