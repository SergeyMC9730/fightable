
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/renderer.h>
#include <fightable/state.h>

void BeginMode2DStacked(Camera2D cam) {
    if (__state.c2dpointer > R2D_STACK_SIZE - 1 || __state.c2dpointer < 0) {
        return BeginMode2D(cam);
    }

    __state.c2dstack[__state.c2dpointer] = cam;

    if (__state.c2dpointer >= 1) EndMode2D();

    __state.c2dpointer++;

    BeginMode2D(cam);
}

void EndMode2DStacked() {
    int current = __state.c2dpointer - 1;

    if (current == 0) {
        __state.c2dpointer--;

        return EndMode2D();
    }

    Camera2D target = __state.c2dstack[current - 1];

    EndMode2D();
    BeginMode2D(target);

    __state.c2dpointer--;
}
