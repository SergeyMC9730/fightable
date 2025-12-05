
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fraylib.h>
#include <fightable/storage.h>
#include <fightable/filesystem.h>
#include <stdlib.h>

void _fLoaderMainPrepareStorage() {
    TraceLog(LOG_INFO, "[LOADER] Preparing writable storage directory");
    _fStoragePrepareWritable();

#ifndef TARGET_ANDROID
    char *assets_dir = "assets";
#else
    char *assets_dir = ".";
#endif
    char *new_assets = _fStorageAppend("assets");

    TraceLog(LOG_INFO, "[LOADER] Copying game assets");
    _fFsDirectoryCopy(assets_dir, new_assets);
    ChangeDirectory(_fStorageGetWritable());

    free(new_assets);
}
