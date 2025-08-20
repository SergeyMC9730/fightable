
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <fightable/intvec.h>

struct fa_clippy {
    IVector2 pos;
    double time;
};

#ifdef __cplusplus
extern "C" {
#endif

// clippy just wants to help you
struct fa_clippy _fAssistantClippyCreate(IVector2 base_pos);

// clippy just wants to update itself
void _fAssistantClippyUpdate(struct fa_clippy *obj);

// clippy just wants to draw itself
void _fAssistantClippyDraw(struct fa_clippy *obj);

#ifdef __cplusplus
}
#endif
