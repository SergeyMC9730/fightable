#include <fightable/gfx.h>

void _fGfxDraw(struct gfx_manager *m) {
    if (!m->fade_v.do_not_display) {
        Vector2 sz = {
            .x = GetRenderWidth(),
            .y = GetRenderHeight()
        };

        DrawRectangle(0, 0, sz.x, sz.y, m->fade_v.current);
    }
}