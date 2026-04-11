
//          Sergei Baigerov 2024 - 2026.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/filesystem.h>
#include <fightable/string.h>

#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <stdio.h>

#ifdef TARGET_WIN32
#include <windows.h>
#include <direct.h>
#define mkdir(PATH, MODE) _mkdir(PATH)
#ifndef S_ISDIR
#define S_ISDIR(m) (((m) & _S_IFDIR) == _S_IFDIR)
#endif
#elif defined(TARGET_UNIX)
#include <dirent.h>
#include <unistd.h>
#endif

unsigned char pfCopyDirectoryNative(const char *src, const char *dest) {
    if (!src || !dest) return 0;

    unsigned int plen = 1024;
    char *src_path = (char *)malloc(plen);
    char *dest_path = (char *)malloc(plen);

#if defined(TARGET_UNIX)
    DIR *dir = opendir(src);
    if (!dir) return 0;

    struct dirent *entry = readdir(dir);

    while ((entry = readdir(dir)) != NULL) {
        if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) {
            continue;
        }

        snprintf(src_path, 1024, "%s/%s", src, entry->d_name);
        snprintf(dest_path, 1024, "%s/%s", dest, entry->d_name);

        struct stat st = {};
        if (stat(src_path, &st) != 0) {
            closedir(dir);
            free(src_path);
            free(dest_path);
            return 0;
        }

        if (S_ISDIR(st.st_mode)) {
            if (
                !_fFsDirectoryCreate(dest_path)
                ||
                !_fFsDirectoryCopy(src_path, dest_path)
            ) {
                closedir(dir);
                free(src_path);
                free(dest_path);
                return 0;
            } else {
                printf("%s: copied dir %s to %s\n", __FILE__, src_path, dest_path);
            }
        } else {
            if (!_fFsFileCopy(src_path, dest_path)) {
                closedir(dir);
                free(src_path);
                free(dest_path);
                return 0;
            } else {
                printf("%s: copied file %s to %s\n", __FILE__, src_path, dest_path);
            }
        }
    }

    closedir(dir);
    free(src_path);
    free(dest_path);

    return 1;
#elif defined(TARGET_WIN32)
    char *search_path = (char *)malloc(plen);
    WIN32_FIND_DATA find_data;

    snprintf(search_path, sizeof(plen), "%s\\*", src);

    HANDLE hFind = FindFirstFile(search_path, &find_data);
    if (hFind == INVALID_HANDLE_VALUE) {
        free(src_path);
        free(dest_path);
        free(search_path);
        return 0;
    }

    int success = 1;

    do {
        if (strcmp(find_data.cFileName, ".") == 0 ||
            strcmp(find_data.cFileName, "..") == 0) {
            continue;
        }

        snprintf(src_path, sizeof(src_path), "%s\\%s", src, find_data.cFileName);
        snprintf(dest_path, sizeof(dest_path), "%s\\%s", dest, find_data.cFileName);

        if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            if (
                !_fFsDirectoryCreate(dest_path)
                ||
                !_fFsDirectoryCopy(src_path, dest_path)
            ) {
                success = 0;
                break;
            }
            printf("%s: copied dir %s to %s\n", __FILE__, src_path, dest_path);
        } else {
            if (!_fFsFileCopy(src_path, dest_path)) {
                success = 0;
                break;
            }
            printf("%s: copied file %s to %s\n", __FILE__, src_path, dest_path);
        }
    } while (FindNextFile(hFind, &find_data) != 0);

    FindClose(hFind);

    success = success && (GetLastError() == ERROR_NO_MORE_FILES);
    if (!success) {
        free(src_path);
        free(dest_path);
        free(search_path);
    }
    return success;
#endif
}
