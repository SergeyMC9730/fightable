#pragma once

#ifdef __cplusplus
extern "C" {
#endif

struct FColor {
    float r;
    float g;
    float b;
    float a;
};

typedef struct FColor FColor;

#include <raylib.h>

FColor _fColorToFloat(Color col);
Color _fFloatToColor(FColor col);

Color _fInvertColor(Color col);
FColor _fInvertFColor(FColor col);

void _fFColorSanitize(FColor *col);

Color _fMixColors(Color a, Color b, float equality);

#ifdef __cplusplus
}
#endif