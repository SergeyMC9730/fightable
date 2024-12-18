#pragma once

#include <raylib.h>

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