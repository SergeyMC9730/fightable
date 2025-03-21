
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

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

#include <fraylib.h>

FColor _fColorToFloat(Color col);
Color _fFloatToColor(FColor col);

Color _fInvertColor(Color col);
FColor _fInvertFColor(FColor col);

void _fFColorSanitize(FColor *col);

Color _fMixColors(Color a, Color b, float equality);

#ifdef __cplusplus
}
#endif
