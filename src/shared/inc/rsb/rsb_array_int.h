
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <rsb/rsb_array_gen.h>

RSB_ARRAY_DEF_GEN(int, _int);

unsigned char RSBContains_int(const rsb_array__int *array, int value);

#ifdef __cplusplus
}
#endif
