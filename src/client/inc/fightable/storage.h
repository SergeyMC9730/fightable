#pragma once

#ifdef __cplusplus
extern "C" {
#endif

const char *_fStorageGetWritable();
void _fStoragePrepareWritable();
int _fStorageMakeDirectory(const char* dirPath);

#ifdef __cplusplus
}
#endif