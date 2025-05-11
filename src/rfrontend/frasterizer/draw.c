
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <X11/Xlib.h>
#include <fraylib.h>

void ClearBackground(Color color) {
#ifdef TARGET_SUPPORTS_X11
    // XDrawRectangle(__frstate.display, __frstate.window, __frstate.fb_context, 0, 0, , unsigned int)
#endif
}
