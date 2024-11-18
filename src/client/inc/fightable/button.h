#pragma once

#include <fightable/intvec.h>
#include <raylib.h>

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

    Color tint;
};

unsigned char _fButtonDraw(struct fbutton *btn);
unsigned char _fButtonDrawSimple(const char *text, IVector2 pos, Color tint);
unsigned char _fSquareButtonDrawSimple(const char *text, IVector2 pos, Color tint);
unsigned char _fSquareButtonDraw(struct fbutton *btn);
#ifdef __cplusplus
}
#endif