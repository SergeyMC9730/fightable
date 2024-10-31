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

#ifdef __cplusplus
}
#endif