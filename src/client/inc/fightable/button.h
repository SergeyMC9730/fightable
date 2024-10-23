#pragma once

#include <fightable/intvec.h>

#ifdef __cplusplus
extern "C" {
#endif

struct fbutton {
    const char *text;
    
    unsigned char howered : 1;
    unsigned char down : 1;
    unsigned char up : 1;

    void *ctx;
    int (*on_click)(struct fbutton *btn);

    IVector2 position;
};

unsigned char _fButtonDraw(struct fbutton *btn);

#ifdef __cplusplus
}
#endif