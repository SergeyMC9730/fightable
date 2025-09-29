
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <rsb/rsb_array_int.h>
#include <rsb/rsb_array_pint.h>

RSB_ARRAY_IMPL_GEN(int, _int);
RSB_ARRAY_IMPL_GEN(int*, _pint);

unsigned char RSBContains_int(const rsb_array__int *array, int value) {
    if (!array) return 0;

    for (unsigned int i = 0; i < array->len; i++) {
        if (array->objects[i] == value) return 1;
    }
    return 0;
}
