
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#if TARGET_RDR_ID == 0
#define TARGET_SUPPORTS_2D
#define TARGET_SUPPORTS_3D
#define TARGET_SUPPORTS_RLGL_2D
#define TARGET_SUPPORTS_RLGL_3D
#include <raylib.h>
#elif TARGET_RDR_ID == 1
#define TARGET_SUPPORTS_2D
#include <frfrontend/frasterizer.h>
#endif
