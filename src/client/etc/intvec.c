#include <raylib.h>
#include <fightable/intvec_math.h>

Vector2 _fImathToVFloat(IVector2 v) {
    Vector2 _v = {(float)v.x, (float)v.y};

    return _v;
}