#include <fightable/sanitizer.h>

float _fSanitizeFloat(float v, float min, float max) {
    if (min > max) return _fSanitizeFloat(v, max, min);

    if (v < min) return min;
    else if (v > max) return max;

    return v;
}