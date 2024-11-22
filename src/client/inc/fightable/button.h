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
    void (*on_click)(struct fbutton *btn);

    IVector2 position;

    Color tint;
};

#define SQRBTN_NONE         0
#define SQRBTN_CHECKBOX     1
#define SQRBTN_EXIT         2
#define SQRBTN_REVCHECKBOX  3

struct fsquare_button {
    unsigned char howered : 1;
    unsigned char down : 1;
    unsigned char up : 1;
    unsigned char flag : 1;

    void *ctx;
    void (*on_click)(struct fsquare_button *btn);

    IVector2 position;
    Color tint;

    unsigned char type;
};
unsigned char _fButtonDraw(struct fbutton *btn);
unsigned char _fButtonDrawSimple(const char *text, IVector2 pos, Color tint);
unsigned char _fSquareButtonDrawSimple(IVector2 pos, Color tint, unsigned char type, unsigned char flag);
unsigned char _fSquareButtonDraw(struct fsquare_button *btn);
#ifdef __cplusplus
}
#endif