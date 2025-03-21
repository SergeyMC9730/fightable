
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/storage.h>
#include <fightable/state.h>

#ifdef TARGET_ANDROID
#include <android_native_app_glue.h>
#endif

const char *_fStorageGetWritable() {
#ifdef TARGET_ANDROID
    return ".";
#elif defined TARGET_UNIX
    return "~/.fightable";
#else
    return ".fightable";
#endif
}

void _fStoragePrepareWritable() {
    const char *path = _fStorageGetWritable();

    MakeDirectory(path);
}
