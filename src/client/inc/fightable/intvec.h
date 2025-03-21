
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct IVector2 {
    int x;
    int y;
} IVector2;

#include <fraylib.h>

Vector2 _fImathToVFloat(IVector2 v);

void _fImathAddI(IVector2 in, IVector2* out);
void _fImathAddV(int ix, int iy, IVector2* out);

void _fImathSubI(IVector2 in, IVector2* out);
void _fImathSubV(int ix, int iy, IVector2* out);

#ifdef __cplusplus
}
#endif
