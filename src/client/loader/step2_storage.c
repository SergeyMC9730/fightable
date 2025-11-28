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
