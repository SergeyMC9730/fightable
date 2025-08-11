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

    return _fCopyStringWithLen(c, strlen(c));
}

void _fCleanupSplittedString(rsb_array__pchar *array) {
    if (!RSBIsValid_pchar(array)) return;

    for (unsigned int i = 0; i < array->len; i++) {
        char *entry = RSBGetAtIndex_pchar(array, i);
        free(entry);
    }

    RSBDestroy_pchar(array);
}

char *_fCopyStringWithLen(const char *c, unsigned int len) {
    char *res = (char *)malloc(len + 1);

    memcpy(res, c, len);
    res[len] = 0;

    return res;
}

int _fGetUtf8AtIndex(const char *utf_string, unsigned int char_index) {
    if (!utf_string) return -1;
    return _fGetUtf8AtIndexWithLen(utf_string, char_index, strlen(utf_string));
}
int _fGetUtf8AtIndexWithLen(const char *utf_string, unsigned int char_index, unsigned int buffer_len) {
    if (!utf_string || buffer_len == 0) return -1;

    unsigned char *p = (unsigned char *)utf_string;
    unsigned char *end = p + buffer_len;
    unsigned int current_index = 0;

    while (p < end && *p && current_index <= char_index) {
        unsigned int codepoint = 0;
        int bytes_to_read = 0;

        if ((*p & 0x80) == 0x00) {
            codepoint = *p;
            bytes_to_read = 1;
        }
        else if ((*p & 0xE0) == 0xC0) {
            if (p + 1 >= end) return -1;
            codepoint = *p & 0x1F;
            bytes_to_read = 2;
        }
        else if ((*p & 0xF0) == 0xE0) {
            if (p + 2 >= end) return -1;
            codepoint = *p & 0x0F;
            bytes_to_read = 3;
        }
        else if ((*p & 0xF8) == 0xF0) {
            if (p + 3 >= end) return -1;
            codepoint = *p & 0x07;
            bytes_to_read = 4;
        }
        else {
            return -1;
        }

        for (int i = 1; i < bytes_to_read; i++) {
            if (p + i >= end) return -1;
            if ((p[i] & 0xC0) != 0x80) {
                return -1;
            }
            codepoint = (codepoint << 6) | (p[i] & 0x3F);
        }

        if (current_index == char_index) {
            if (codepoint > 0x10FFFF) return -1;
            if (codepoint >= 0xD800 && codepoint <= 0xDFFF) return -1;
            return (int)codepoint;
        }

        current_index++;
        p += bytes_to_read;
    }

    return -1;
}
