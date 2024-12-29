#include <fightable/level.h>
#include <fightable/state.h>
#include <math.h>

void _fLevelLightSourceDraw(struct flevel* env, struct flevel_light_source* source) {
	if (!env || !source) return;

    double sz = (sinl(__state.time * 1.5f) + 1.5f) / 3.f;

    int cx = source->pos.x + (env->tilemap->tile_size.x / 2);
    int cy = source->pos.y + (env->tilemap->tile_size.y / 2);

    DrawCircleGradient(cx, cy, sz * env->tilemap->tile_size.x * 2, source->tint, BLANK);
}