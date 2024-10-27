#include <fightable/state.h>
#include <fightable/gfx.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

void _fGfxShake(struct gfx_manager *m, float strength) {
    m->shake_v.max += 0.15f;
    m->shake_v.s += strength;
}
void _fGfxShakeConst(struct gfx_manager *m, float strength) {
    m->shake_v.time = 0;
    m->shake_v.max = 0.15f;
    m->shake_v.x = 0;
    m->shake_v.y = 0;
    m->shake_v.s = strength;
}

void _fGfxShakeUpdate(struct gfx_manager *m) {
    if (m->shake_v.time > m->shake_v.max || m->shake_v.max == 0.f) {
        m->shake_v.x = 0;
        m->shake_v.y = 0;
        m->shake_v.s = 0;
        m->shake_v.max = 0;
        m->shake_v.time = 0;
        
        return;
    }

    double delta = GetFrameTime();
    double speed = 1.f / m->shake_v.max;

    double strength_power = 1.f - pow(m->shake_v.time * speed, 3.f);

    double rng1 = (double)(rand() % 2);
    double rng2 = (double)(rand() % 2);

    unsigned char reversed[2] = {rand() % 2, rand() % 2};

    m->shake_v.x = rng1 * strength_power * m->shake_v.s * ((reversed[0]) ? -1.f : 1.f);
    m->shake_v.y = rng2 * strength_power * m->shake_v.s * ((reversed[1]) ? -1.f : 1.f);

    m->shake_v.time += delta;
    
    // printf("%f | %f -> %f | %f | %f %f -> %f:%f | %d %d\n",
    //     speed,
    //     m->shake_v.time, m->shake_v.time * speed,
    //     strength_power,
    //     rng1, rng2,
    //     m->shake_v.x, m->shake_v.y,
    //     reversed[0], reversed[1]
    // );
}