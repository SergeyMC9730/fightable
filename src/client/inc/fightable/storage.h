
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

const char *_fStorageGetWritable();
void _fStoragePrepareWritable();
// free memory after use
char *_fStorageFind(const char *path);
// free memory after use
char *_fStorageAppend(const char *path);

#ifdef __cplusplus
}
#endif
