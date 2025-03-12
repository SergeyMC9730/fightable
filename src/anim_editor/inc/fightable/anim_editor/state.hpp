
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <nt5emul/renderer_animation.h>
#include <stddef.h>
#include <raylib.h>

namespace fightable {
    struct state {
        RLFont unifont1 = {};
        renderer_animation *anim = NULL;
    };
}

extern fightable::state __state;
