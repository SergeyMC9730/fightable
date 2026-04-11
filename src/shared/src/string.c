
//          Sergei Baigerov 2024 - 2026.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <stdlib.h>
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
    if (!c || !len) return NULL;

    char *dest = (char *)malloc(len + 1);
    memcpy(dest, c, len);
    dest[len] = 0;

    return dest;
}

int _fGetUtf8AtIndex(const char *utf_string, unsigned int char_index) {
    return utf_string != NULL ? _fGetUtf8AtIndexWithLen(utf_string, char_index, strlen(utf_string)) : -1;
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

unsigned int _fGetUtf8ByteOffset(const char *utf_string, unsigned int char_index) {
    return utf_string != NULL ? _fGetUtf8ByteOffsetWithLen(utf_string, char_index, strlen(utf_string)) : 0;
}
unsigned int _fGetUtf8ByteOffsetWithLen(const char *utf_string, unsigned int char_index, unsigned int buffer_len) {
    if (!utf_string || buffer_len == 0) return 0;

    unsigned int byte_offset = 0;
    unsigned int current_char_index = 0;

    while (byte_offset < buffer_len && utf_string[byte_offset] != 0 && current_char_index < char_index) {
        unsigned char c = utf_string[byte_offset];
        int bytes_in_char = 1;

        if ((c & 0xE0) == 0xC0) bytes_in_char = 2;
        else if ((c & 0xF0) == 0xE0) bytes_in_char = 3;
        else if ((c & 0xF8) == 0xF0) bytes_in_char = 4;

        if (byte_offset + bytes_in_char > buffer_len) {
            return 0;
        }

        for (int i = 1; i < bytes_in_char; i++) {
            if ((utf_string[byte_offset + i] & 0xC0) != 0x80) {
                return 0;
            }
        }

        byte_offset += bytes_in_char;
        current_char_index++;
    }

    return byte_offset;
}

unsigned char _fStringEndsWith(const char *str, const char *substr) {
    if (!str || !substr) return 0;

    unsigned int str_len = strlen(str);
    unsigned int substr_len = strlen(substr);

    if (substr_len > str_len) return _fStringEndsWith(substr, str);
    return strcmp(str + str_len - substr_len, substr) == 0;
}
