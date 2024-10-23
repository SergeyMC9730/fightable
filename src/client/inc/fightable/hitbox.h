#pragma once

#include <raylib.h>

#ifdef __cplusplus
extern "C" {
#endif

float _fHitboxClipXCollide(Rectangle *h1, Rectangle *h2, float x);
float _fHitboxClipYCollide(Rectangle *h1, Rectangle *h2, float x);
unsigned char _fHitboxIntersects(Rectangle *h1, Rectangle *h2);

void _fHitboxMove(Rectangle *h1, float x, float y);
Rectangle _fHitboxExpand(Rectangle *h, float x, float y);

#ifdef __cplusplus
}
#endif