
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <fightable/intvec.h>

#ifdef __cplusplus
extern "C" {
#endif

void _fRectDraw(RLRectangle r, Color grad_top, Color grad_bottom, Color container);
RLRectangle _fRectAbsSize(RLRectangle r);

#ifdef __cplusplus
}
#endif
