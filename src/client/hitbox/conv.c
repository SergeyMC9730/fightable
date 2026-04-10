
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/hitbox.h>

RLRectangle _fHitboxToRect(fhitbox hitbox) {
    return hitbox;
}
fhitbox _fHitboxFromRect(RLRectangle rect) {
    return rect;
}

Vector2 _fHitboxPositionFromHitbox(fhitbox hitbox) {
    return (Vector2){hitbox.x, hitbox.y};
}
