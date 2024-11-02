#include <fightable/hitbox.h>
#include <math.h>

float _fHitboxClipYCollide(fhitbox *h1, fhitbox *h2, float ya) {
    if (h2->x + h2->width <= h1->x || h2->x >= h1->x + h1->width) {
        return ya;
    }

    if (ya > 0.0f && h2->y + h2->height <= h1->y)
        ya = fmin(ya, h1->y - (h2->y + h2->height));

    if (ya < 0.0f && h2->y >= h1->y + h1->height)
        ya = fmax(ya, (h1->y + h1->height) - h2->y);
        
    // printf("%f \n, %f \n, %f \n, %f \n", h1->x, h1->y, h1->width, h1->height);
    // printf("%f \n, %f \n, %f \n, %f \n", h2->x, h2->y, h2->width, h2->height);
    return ya;
}