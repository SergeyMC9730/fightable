
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <nbt.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NBT_IMPLEMENTATION
typedef struct {
    uint8_t* buffer;
    size_t buffer_offset;
} nbt__read_stream_t;
typedef struct {
    uint8_t* buffer;
    size_t offset;
    size_t size;
    size_t alloc_size;
} nbt__write_stream_t;
#endif

extern nbt_tag_t* nbt__parse(nbt__read_stream_t* stream, int parse_name, nbt_tag_type_t override_type);
extern void nbt__write_tag(nbt__write_stream_t* stream, nbt_tag_t* tag, int write_name, int write_type);

nbt_tag_t *nbt_set_tag_name_easy(nbt_tag_t *tag, const char *name);

size_t nbt_reader_default(void* userdata, uint8_t* data, size_t size);
size_t nbt_writer_default(void* userdata, uint8_t* data, size_t size);

size_t nbt_reader_default_buf(void* userdata, uint8_t* data, size_t size);

#ifdef __cplusplus
}
#endif
