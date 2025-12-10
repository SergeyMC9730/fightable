
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/nbt_tools.h>
#include <string.h>

nbt_tag_t *nbt_set_tag_name_easy(nbt_tag_t *tag, const char *name) {
    if (tag && name) {
        nbt_set_tag_name(tag, name, strlen(name) + 1);
    }

    return tag;
}

size_t nbt_reader_default(void* userdata, uint8_t* data, size_t size) {
    return fread(data, 1, size, userdata);
}
size_t nbt_writer_default(void* userdata, uint8_t* data, size_t size) {
    return fwrite(data, 1, size, userdata);
}

size_t nbt_reader_default_buf(void* stream_orig, uint8_t* data, size_t size) {
    memcpy(data, stream_orig, size);
    return size;
}
