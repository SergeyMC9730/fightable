#include <fightable/gfx.h>

extern void _fGfxShakeUpdate(struct gfx_manager *m);
extern void _fGfxFadeUpdate(struct gfx_manager *m);

void _fGfxUpdate(struct gfx_manager *m) {
    _fGfxShakeUpdate(m);
    _fGfxFadeUpdate(m);
}