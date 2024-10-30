#include <fightable/storage.h>
#include <fightable/state.h>

#ifdef TARGET_ANDROID
#include <android_native_app_glue.h>
#endif

const char *_fStorageGetWritable() {
#ifdef TARGET_ANDROID
    return ".";
#elif defined TARGET_UNIX
    return ".fightable";
#else
    return ".fightable";
#endif
}

#include <raylib.h>

void _fStoragePrepareWritable() {
    const char *path = _fStorageGetWritable();

    MakeDirectory(path);
}