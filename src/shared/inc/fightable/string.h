#pragma once

#include <rsb/rsb_array_pchar.h>

#ifdef __cplusplus
extern "C" {
#endif

char *_fCopyString(const char *c);
char *_fCopyStringWithLen(const char *c, unsigned int len);

rsb_array__pchar *_fSplitString(const char *string, const char delimiter);
void _fCleanupSplittedString(rsb_array__pchar *array);

#ifdef __cplusplus
}
#endif
