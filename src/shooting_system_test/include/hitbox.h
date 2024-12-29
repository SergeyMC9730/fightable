#pragma once

#include <raylib.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif
typedef struct fhitbox {
    float x;
    float y;
    float width;
    float height;
} fhitbox;

float _fHitboxClipXCollide(struct fhitbox *h1, struct fhitbox *h2, float x);
float _fHitboxClipYCollide(struct fhitbox *h1, struct fhitbox *h2, float x);
unsigned char _fHitboxIntersects(RLRectangle *h1, RLRectangle *h2);

void _fHitboxMove(RLRectangle *h1, float x, float y);
RLRectangle _fHitboxExpand(RLRectangle *h, float x, float y);

#ifdef __cplusplus
}
#endif