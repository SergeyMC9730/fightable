
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/filesystem.h>

#include <stdio.h>
#include <stdlib.h>

// path and destination_path should also contain filename. returns 1 on success
unsigned char _fFsFileCopy(const char *path, const char *destination_path) {
    if (!path || !destination_path) return 0;

    FILE *src_file = fopen(path, "rb");
    if (!src_file) return 0;

    FILE *dest_file = fopen(destination_path, "wb");
    if (!dest_file) {
        fclose(src_file);
        return 0;
    }

    void *buffer = malloc(512);
    size_t bytes = 0;
    int success = 1;

    while ((bytes = fread(buffer, 1, sizeof(buffer), src_file)) > 0) {
        if (fwrite(buffer, 1, bytes, dest_file) != bytes) {
            success = 0;
            break;
        }
    }

    fclose(src_file);
    fclose(dest_file);

    free(buffer);

    return success;
}
