#pragma once

#include <raylib.h>

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
unsigned char _fHitboxIntersects(Rectangle *h1, Rectangle *h2);

void _fHitboxMove(Rectangle *h1, float x, float y);
fhitbox _fHitboxExpand(fhitbox *h, float x, float y);

#ifdef __cplusplus
}
#endif