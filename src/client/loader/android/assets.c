
//          Sergei Baigerov 2024 - 2026.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#ifdef TARGET_ANDROID

#include <fightable/loader.h>
#include <android/asset_manager.h>
#include <android_native_app_glue.h>
#include <fightable/storage.h>
#include <fightable/state.h>
#include <fightable/filesystem.h>

unsigned char _fLoaderMainCopyFromApk(const char *asset_path, const char *dest_path) {
    if (!__state.system || !__state.system->activity) {
        TraceLog(LOG_ERROR, "[LOADER ANDROID] Android context is not loaded");
        return 0;
    }

    AAssetManager* mgr = __state.system->activity->assetManager;
    if (!mgr) {
        TraceLog(LOG_ERROR, "[LOADER ANDROID] Asset Manager is not loaded");
        return 0;
    }

    AAssetDir* dir = AAssetManager_openDir(mgr, asset_path);
    if (!dir) {
        TraceLog(LOG_ERROR, "[LOADER ANDROID] Cannot open asset directory %s", asset_path);
        return 0;
    }

    const char* filename;
    int success = 1;

    char *src_full = (char *)malloc(512);
    char *dest_full = (char *)malloc(512);

    while ((filename = AAssetDir_getNextFileName(dir)) != NULL) {
        snprintf(src_full, 512, "%s/%s", asset_path, filename);
        snprintf(dest_full, 512, "%s/%s", dest_path, filename);

        AAsset* asset = AAssetManager_open(mgr, src_full, AASSET_MODE_BUFFER);

        if (asset) {
            void *data = AAsset_getBuffer(asset);
            off_t size = AAsset_getLength(asset);

            FILE *dest_file = fopen(dest_full, "wb");
            if (dest_file) {
                size_t written = fwrite(data, 1, size, dest_file);
                if (written == size) {
                    TraceLog(LOG_INFO, "[LOADER ANDROID] Copied asset from %s to %s (%ld bytes)", src_full, dest_full, size);
                } else {
                    TraceLog(LOG_ERROR, "[LOADER ANDROID] Failed to write %s", dest_full);
                    success = 0;
                }
                fclose(dest_file);
            } else {
                TraceLog(LOG_ERROR, "[LOADER ANDROID] Cannot create destination file %s", dest_full);
                success = 0;
            }

            AAsset_close(asset);
        } else {
            char *sub_asset_path = (char *)malloc(512);
            char *sub_dest_path = (char *)malloc(512);

            snprintf(sub_asset_path, 512, "%s/%s", asset_path, filename);
            snprintf(sub_dest_path, 512, "%s/%s", dest_path, filename);

            _fFsDirectoryCreate(sub_dest_path);

            if (!_fLoaderMainCopyFromApk(sub_asset_path, sub_dest_path)) {
                success = 0;
            }

            free(sub_asset_path);
            free(sub_dest_path);
        }
    }

    AAssetDir_close(dir);

    free(src_full);
    free(dest_full);

    return success;
}

void _fLoaderMainLoadAssets() {
    const char* writable = _fStorageGetWritable();
    char *assets_dest = (char *)malloc(512);
    snprintf(assets_dest, 512, "%s/assets", writable);

    TraceLog(LOG_INFO, "[LOADER ANDROID] Copying assets from APK to %s", assets_dest);

    if (!_fFsDirectoryCreate(assets_dest)) {
        TraceLog(LOG_ERROR, "[LOADER ANDROID] Failed to create assets directory");
        free(assets_dest);
        return;
    }

    _fLoaderMainCopyFromApk("assets", assets_dest);
    free(assets_dest);
}

#endif
