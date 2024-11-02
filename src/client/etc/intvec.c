#include <raylib.h>
#include <fightable/intvec_math.h>

Vector2 _fImathToVFloat(IVector2 v) {
    Vector2 _v = {(float)v.x, (float)v.y};

    return _v;
}

void _fImathAddI(IVector2 in, IVector2* out) {
    _fImathAddV(in.x, in.y, out);
}
void _fImathAddV(int ix, int iy, IVector2* out) {
    if (!out) return;

    out->x += ix;
    out->y += iy;
}

void _fImathSubI(IVector2 in, IVector2* out) {
    _fImathSubV(in.x, in.y, out);
}
void _fImathSubV(int ix, int iy, IVector2* out) {
    _fImathAddV(-ix, -iy, out);
}