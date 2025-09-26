
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#if defined(TARGET_WIN32)
#define FS_PATH_SEPARATOR '\\'
#elif defined(TARGET_UNIX)
#define FS_PATH_SEPARATOR '/'
#endif

#ifdef __cplusplus
extern "C" {
#endif

// copies directory recursively. returns 1 on success
unsigned char _fFsDirectoryCopy(const char *path, const char *destination_path);

// creates directory. returns 1 on success
unsigned char _fFsDirectoryCreate(const char *path);

// path and destination_path should also contain filename. returns 1 on success
unsigned char _fFsFileCopy(const char *path, const char *destination_path);

#ifdef __cplusplus
}
#endif
