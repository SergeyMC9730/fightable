
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/storage.h>
#include <fightable/state.h>
#include <stdio.h>

#ifdef TARGET_ANDROID
#include <android_native_app_glue.h>
#endif

char fTempStorageBuffer[512] = {0};

const char *_fStorageGetWritable() {
#ifdef TARGET_ANDROID
    if (!fTempStorageBuffer[0] && __state.system && __state.system->activity) {
        int attempt = 1;
        if (__state.system->activity->internalDataPath) {
            snprintf(fTempStorageBuffer, sizeof(fTempStorageBuffer), "%s", __state.system->activity->internalDataPath);
        } else if (__state.system->activity->externalDataPath) {
            attempt = 2;
            snprintf(fTempStorageBuffer, sizeof(fTempStorageBuffer), "%s", __state.system->activity->externalDataPath);
        } else {
            attempt = 3;
            JNIEnv* env = __state.system->activity->env;
            jobject activity = __state.system->activity->clazz;

            if (env && activity) {
                // Get the internal storage directory
                jclass class_activity = (*env)->GetObjectClass(env, activity);
                jmethodID method_getFilesDir = (*env)->GetMethodID(env, class_activity, "getFilesDir", "()Ljava/io/File;");

                if (method_getFilesDir) {
                    jobject file = (*env)->CallObjectMethod(env, activity, method_getFilesDir);

                    if (file) {
                        jmethodID method_getPath = (*env)->GetMethodID(env, (*env)->GetObjectClass(env, file), "getPath", "()Ljava/lang/String;");

                        if (method_getPath) {
                            jstring path_str = (*env)->CallObjectMethod(env, file, method_getPath);
                            const char* path = (*env)->GetStringUTFChars(env, path_str, NULL);

                            if (path) {
                                snprintf(fTempStorageBuffer, sizeof(fTempStorageBuffer), "%s", path);
                                (*env)->ReleaseStringUTFChars(env, path_str, path);
                            }
                        }
                    }
                }
            }
        }

        if (!fTempStorageBuffer[0]) {
            attempt = 4;
            snprintf(fTempStorageBuffer, sizeof(fTempStorageBuffer), "/data/data/dogotrigger.fightable/files");
        }

        TraceLog(LOG_INFO, "Tried to determine storage path: %s (attempt=%d)", fTempStorageBuffer, attempt);
    }
    return (const char *)fTempStorageBuffer;
#elif defined(TARGET_UNIX)
    char *home = getenv("HOME");
    if (!home) {
        return ".fightable";
    } else {
        snprintf(fTempStorageBuffer, 512, "%s/.fightable", home);
        return (const char *)fTempStorageBuffer;
    }
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
    return FileExists(file);
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

// free memory after use
char *_fStorageAppend(const char *path) {
    const char *storage = _fStorageGetWritable();

    size_t buf_size = strlen(storage) + 1 + strlen(path) + 1;
    char *buf = (char *)malloc(buf_size);

    snprintf(buf, buf_size, "%s/%s", storage, path);

    return buf;
}
