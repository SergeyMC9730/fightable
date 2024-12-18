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

    
}

#include <stdlib.h>

// keeping this code until raylib update
// this code has been taken from https://github.com/raysan5/raylib/blob/master/src/rcore.c

#if defined(_WIN32)
#include <io.h>                 // Required for: _access() [Used in FileExists()]
#include <direct.h>             // Required for: _getch(), _chdir(), _mkdir()
#define GETCWD _getcwd          // NOTE: MSDN recommends not to use getcwd(), chdir()
#define CHDIR _chdir
#define MKDIR(dir) _mkdir(dir)
#else
#include <unistd.h>             // Required for: getch(), chdir(), mkdir(), access()
#define GETCWD getcwd
#define CHDIR chdir
#define MKDIR(dir) mkdir(dir, 0777)
#endif

int _fStorageMakeDirectory(const char* dirPath) {
    if ((dirPath == NULL) || (dirPath[0] == '\0')) return 1; // Path is not valid
    if (DirectoryExists(dirPath)) return 0; // Path already exists (is valid)

    // Copy path string to avoid modifying original
    int len = (int)strlen(dirPath) + 1;
    char* pathcpy = (char*)RL_CALLOC(len, 1);
    memcpy(pathcpy, dirPath, len);

    // Iterate over pathcpy, create each subdirectory as needed
    for (int i = 0; (i < len) && (pathcpy[i] != '\0'); i++)
    {
        if (pathcpy[i] == ':') i++;
        else
        {
            if ((pathcpy[i] == '\\') || (pathcpy[i] == '/'))
            {
                pathcpy[i] = '\0';
                if (!DirectoryExists(pathcpy)) MKDIR(pathcpy);
                pathcpy[i] = '/';
            }
        }
    }

    // Create final directory
    if (!DirectoryExists(pathcpy)) MKDIR(pathcpy);

    RL_FREE(pathcpy);

    return 0;
}