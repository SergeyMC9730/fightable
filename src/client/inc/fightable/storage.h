#pragma once

#ifdef __cplusplus
extern "C" {
#endif

const char *_fStorageGetWritable();
void _fStoragePrepareWritable();
// free memory after use
char *_fStorageFind(const char *path);

#ifdef __cplusplus
}
#endif
