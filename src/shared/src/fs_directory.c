
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/filesystem.h>
#include <fightable/string.h>

#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#ifdef TARGET_WIN32
#include <windows.h>
#include <direct.h>
#define mkdir(PATH, MODE) _mkdir(PATH)
#elif defined(TARGET_UNIX)
#include <dirent.h>
#include <unistd.h>
#endif

unsigned char pfCopyDirectoryNative(const char *src, const char *dest);

// creates directory
unsigned char _fFsDirectoryCreate(const char *path) {
    if (!path) return 0;

    char *temp = _fCopyString(path);
    char *p = temp;

#ifdef PLATFORM_WIN32
    if (strlen(temp) > 1 && temp[1] == ':') {
        p = temp + 2;
    }
#endif

    while (*p) {
        if (*p == FS_PATH_SEPARATOR) {
            *p = '\0';
            if (strlen(temp) > 0) {
                mkdir(temp, 0777);
            }
            *p = FS_PATH_SEPARATOR;
        }
        p++;
    }

    free(temp);
    mkdir(path, 0777);

    return 1;
}

// copies directory recursively. returns 1 on success
unsigned char _fFsDirectoryCopy(const char *path, const char *destination_path) {
    if (!path || !destination_path) return 0;

    struct stat st;
    if (stat(path, &st) != 0 || !S_ISDIR(st.st_mode)) {
        return 0;
    }

    if (!_fFsDirectoryCreate(destination_path)) {
        return 0;
    }

    return pfCopyDirectoryNative(path, destination_path);
}
