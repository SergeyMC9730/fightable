
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/entity.h>

float _fEntityGetDelta(struct fentity* entity) {
    if (!entity || entity->custom_delta <= 0.f) return GetFrameTime();
    return entity->custom_delta;
}
