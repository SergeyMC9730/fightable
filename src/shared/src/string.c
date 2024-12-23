#include <fightable/string.h>
#include <string.h>
#include <rsb/rsb_array_char.h>

rsb_array__pchar *_fSplitString(const char *string, const char delimiter) {
    rsb_array__pchar *array = RSBCreateArray_pchar();
    unsigned int len = strlen(string);

    unsigned int j = 0;
    
    rsb_array__char *temp = RSBCreateArray_char();

    for (unsigned int i = 0; i < len; i++) {
        const char c = string[i];
        if (c == delimiter) {
            RSBAddElement_char(temp, 0);

            char *str = _fCopyString((const char *)temp->objects);
            RSBAddElement_pchar(array, str);

            RSBDestroy_char(temp);
            temp = RSBCreateArray_char();
        } else {
            RSBAddElement_char(temp, c);
        }
    }

    RSBAddElement_char(temp, 0);

    char *str = _fCopyString((const char *)temp->objects);
    RSBAddElement_pchar(array, str);

    RSBDestroy_char(temp);

    return array;
}

char *_fCopyString(const char *c) {
    if (!c) return NULL;

    unsigned int len = strlen(c) + 1;
    char *res = (char *)malloc(len);

    memcpy(res, c, len);
    res[len - 1] = 0;

    return res;
}

void _fCleanupSplittedString(rsb_array__pchar *array) {
    if (!RSBIsValid_pchar(array)) return;

    for (unsigned int i = 0; i < array->len; i++) {
        char *entry = RSBGetAtIndex_pchar(array, i);
        free(entry);
    }

    RSBDestroy_pchar(array);
}