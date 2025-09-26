
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <fraylib.h>

#ifdef __cplusplus
extern "C" {
#endif

double _fDistPointToCircle(Vector2 point, Vector2 circle_center, double circle_radius);
double _fDistPointToPoint(Vector2 point_a, Vector2 point_b);

#ifdef __cplusplus
}
#endif
