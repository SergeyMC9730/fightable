
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/sanitizer.h>

float _fSanitizeFloat(float v, float min, float max) {
    if (min > max) return _fSanitizeFloat(v, max, min);

    if (v < min) return min;
    else if (v > max) return max;

    return v;
}

double _fSanitizeDouble(double v, double min, double max) {
    if (min > max) return _fSanitizeDouble(v, max, min);

    if (v < min) return min;
    else if (v > max) return max;

    return v;
}
