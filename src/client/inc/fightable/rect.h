#pragma once

#include <raylib.h>
#include <fightable/intvec.h>

#ifdef __cplusplus
extern "C" {
#endif

void _fRectDraw(RLRectangle r, Color grad_top, Color grad_bottom, Color container);
RLRectangle _fRectAbsSize(RLRectangle r);

#ifdef __cplusplus
}
#endif
