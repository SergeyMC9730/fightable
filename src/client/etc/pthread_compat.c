
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/pthread_compat.h>
#include <string.h>

unsigned char _fComparePthreads(PTHREAD_TYPE a, PTHREAD_TYPE b) {
    return memcmp(&a, &b, sizeof(PTHREAD_TYPE)) == 0;
}
unsigned char _fComparePthreadAndEmptyThread(PTHREAD_TYPE a) {
    PTHREAD_TYPE empty = (PTHREAD_TYPE){};
    return _fComparePthreads(a, empty);
}
