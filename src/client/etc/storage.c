
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
    MakeDirectory(_fStorageGetWritable());
}

#include <string.h>
#include <stdio.h>

unsigned char _fStorageFileExists(const char *file) {
    if (!file) return 0;
    FILE *p = fopen(file, "rb");
    if (!p) return 0;
    fclose(p);
    return 1;
}

char *_fStorageFind(const char *path) {
    if (!path) goto _ret;

    unsigned int len = strlen(path) + 1;
    char *buf = NULL;
    const char *storage_path = path;
    const char *pattern = "%s";

    if (_fStorageFileExists(path)) goto _begin;

    storage_path = _fStorageGetWritable();
    len += strlen(storage_path) + 1;
    pattern = "%s/%s";
_begin:
    buf = (char *)MemAlloc(len);
    snprintf(buf, len, pattern, storage_path, path);
_ret:
    return buf;
}
