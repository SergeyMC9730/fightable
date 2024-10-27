#pragma once

#ifdef __cplusplus
extern "C" {
#endif

struct gfx_manager {
    struct shake {
        double time;
        double max;
        double s;

        double x;
        double y;
    } shake_v;
};

void _fGfxShake(struct gfx_manager *m, float strength);
void _fGfxShakeConst(struct gfx_manager *m, float strength);

void _fGfxInvBorders(struct gfx_manager *m);

void _fGfxUpdate(struct gfx_manager *m);

#ifdef __cplusplus
}
#endif

