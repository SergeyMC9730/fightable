
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <fraylib.h>

struct fslider {
    RLRectangle rect;
    float btn_relative_pos;

    float progress;
    float scaling;
    float movable_width;

    Color tint;

    unsigned char moving : 1;
};

#ifdef __cplusplus
extern "C" {
#endif

void _fSliderDraw(struct fslider *slider);

#ifdef __cplusplus
}
#endif
