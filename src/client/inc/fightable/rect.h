#pragma once

#include <raylib.h>
#include <fightable/intvec.h>

#ifdef __cplusplus
extern "C" {
#endif

void _fRectDraw(Rectangle r, Color grad_top, Color grad_bottom, Color container);

#ifdef __cplusplus
}
#endif