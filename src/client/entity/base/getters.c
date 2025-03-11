#include "raylib.h"
#include <fightable/entity.h>

float _fEntityGetDelta(struct fentity* entity) {
    if (!entity || entity->custom_delta <= 0.f) return GetFrameTime();
    return entity->custom_delta;
}
