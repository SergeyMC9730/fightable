#include <fightable/hitbox.h>
#include <math.h>

float _fHitboxClipXCollide(struct fhitbox *h1, struct fhitbox *h2, float xa) {
    if (h2->y + h2->height <= h1->y || h2->y >= h1->y + h1->height) {
        return xa;
    }

    if (xa > 0.0f && h2->x + h2->width <= h1->x)
        xa = fmin(h1->x - (h2->x + h2->width), xa);

    if (xa < 0.0f && h2->x >= h1->x + h1->width)
        xa = fmax(xa, (h1->x + h1->width) - h2->x);

    // printf("%f \n, %f \n, %f \n, %f \n", h1->x, h1->y, h1->width, h1->height);
    // printf("%f \n, %f \n, %f \n, %f \n", h2->x, h2->y, h2->width, h2->height);
    return xa;
}