
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <fraylib.h>

#ifdef __cplusplus
extern "C" {
#endif

//typedef struct fhitbox {
//    float x;
//    float y;
//    float width;
//    float height;
//} fhitbox;

typedef RLRectangle fhitbox;

float _fHitboxClipXCollide(fhitbox *h1, fhitbox *h2, float x);
float _fHitboxClipYCollide(fhitbox *h1, fhitbox *h2, float x);
unsigned char _fHitboxIntersects(fhitbox *h1, fhitbox *h2);

void _fHitboxMove(fhitbox *h1, float x, float y);
fhitbox _fHitboxExpand(fhitbox *h, float x, float y);

#ifdef __cplusplus
}
#endif
