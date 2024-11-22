#pragma once

#include <fightable/button.h>
#include <fightable/slider.h>

struct fconfig {
    struct fsquare_button vsync_btn;
    unsigned char vsync_flag;

    struct fslider volume_slider;
};