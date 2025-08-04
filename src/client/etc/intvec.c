
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/intvec.h>

Vector2 _fImathToVFloat(IVector2 v) {
    Vector2 _v = {(float)v.x, (float)v.y};

    return _v;
}
IVector2 _fVFloatToI(Vector2 v) {
    IVector2 _v = {(int)v.x, (int)v.y};

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
