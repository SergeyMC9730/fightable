#pragma once

#include <fightable/button.h>
#include <fightable/slider.h>

struct fconfig {
    struct fsquare_button vsync_btn;
    unsigned char vsync_flag;

    unsigned char fullscreen_flag;
    struct fsquare_button fullscreen_btn;
    
    struct fslider volume_slider;
};

void _fConfigSave(struct fconfig *config);
void _fConfigRead(struct fconfig* config);
void _fConfigInit(struct fconfig* config);