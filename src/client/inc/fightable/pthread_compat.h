
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#if defined(TARGET_WIN32)
#include <pthread.h>
#define PTHREAD_TYPE pthread_t
#elif defined(TARGET_UNIX)
#include <pthread.h>
#define PTHREAD_TYPE pthread_t
#endif

unsigned char _fComparePthreads(PTHREAD_TYPE a, PTHREAD_TYPE b);
unsigned char _fComparePthreadAndEmptyThread(PTHREAD_TYPE a);
