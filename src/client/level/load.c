
//          Sergei Baigerov 2024 - 2026.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/level.h>
#include <fightable/block.h>
#include <fightable/block_library.h>
#include <fightable/state.h>
#include <fightable/notif_mgr.h>
#include <fightable/nbt_tools.h>
#include <rsb/rsb_array_int.h>
#include <stdio.h>
#include <fightable/translation.h>

static void print_nbt_tree(nbt_tag_t* tag, int indentation) {
  for (int i = 0; i < indentation; i++) {
    printf(" ");
  }

  if (tag->name) {
    printf("%s: ", tag->name);
  }

  switch (tag->type) {
    case NBT_TYPE_END: {
      printf("[end]");
      break;
    }
    case NBT_TYPE_BYTE: {
      printf("%hhd", tag->tag_byte.value);
      break;
    }
    case NBT_TYPE_SHORT: {
      printf("%hd", tag->tag_short.value);
      break;
    }
    case NBT_TYPE_INT: {
      printf("%d", tag->tag_int.value);
      break;
    }
    case NBT_TYPE_LONG: {
      printf("%ld", tag->tag_long.value);
      break;
    }
    case NBT_TYPE_FLOAT: {
      printf("%f", tag->tag_float.value);
      break;
    }
    case NBT_TYPE_DOUBLE: {
      printf("%f", tag->tag_double.value);
      break;
    }
    case NBT_TYPE_BYTE_ARRAY: {
      printf("[byte array]");
      break;
      for (size_t i = 0; i < tag->tag_byte_array.size; i++) {
        printf("%hhd ", tag->tag_byte_array.value[i]);
      }
      break;
    }
    case NBT_TYPE_STRING: {
      printf("%s", tag->tag_string.value);
      break;
    }
    case NBT_TYPE_LIST: {
      printf("\n");
      for (size_t i = 0; i < tag->tag_list.size; i++) {
        print_nbt_tree(tag->tag_list.value[i], indentation + tag->name_size + 2);
      }
      break;
    }
    case NBT_TYPE_COMPOUND: {
      printf("\n");
      for (size_t i = 0; i < tag->tag_compound.size; i++) {
        print_nbt_tree(tag->tag_compound.value[i], indentation + tag->name_size + 2);
      }
      break;
    }
    case NBT_TYPE_INT_ARRAY: {
      printf("[int array]");
      break;
      for (size_t i = 0; i < tag->tag_int_array.size; i++) {
        printf("%d ", tag->tag_int_array.value[i]);
      }
      break;
    }
    case NBT_TYPE_LONG_ARRAY: {
      printf("[long array]");
      break;
      for (size_t i = 0; i < tag->tag_long_array.size; i++) {
        printf("%ld ", tag->tag_long_array.value[i]);
      }
      break;
    }
    default: {
      printf("[error]");
    }
  }

  printf("\n");
}

struct flevel* _fLevelLoadFromFile(const char* filename) {
    int len = 0;
    unsigned char* data = LoadFileData(filename, &len);

    if (!data) return NULL;
    if (len < 10) {
        MemFree(data);
        return NULL;
    }

    short format = ((short*)data)[0];
    TraceLog(LOG_INFO, "Level format: %d", format);

    struct flevel* level = NULL;

    switch (format) {
    case 2: {
        TraceLog(LOG_INFO, "Loading outdated level (2)");

        unsigned short width = ((short*)data)[1];
        unsigned short height = ((short*)data)[2];

        unsigned char parse_bitfield = 0;

        TraceLog(LOG_INFO, "Level size: %dx%d", width, height);

        if (width == 0xFFFF) {
            parse_bitfield = 1;
            TraceLog(LOG_INFO, "Parsing bitfields for outdated level");
        }

        unsigned int objects = *(unsigned int*)(data + 6) % 3000000;

        TraceLog(LOG_INFO, "Objects: %d", objects);

        level = _fLevelLoadTest(__state.tilemap, (IVector2) { 28, 4 });

        free(level->objects);

        level->width = width;
        level->height = height;
        level->data_size = objects;
        level->objects = (struct fblock*)MemAlloc(sizeof(struct fblock) * level->data_size);

        unsigned int rid = 1;

        for (unsigned int i = 0; i < objects; i++) {
            unsigned int offset = 10 + (9 * i);
            if (offset >= len) {
                TraceLog(LOG_WARNING, "Unexpected EOF at %d", offset);
                break;
            }

            unsigned char* ref = data + offset;
            short* sref = (short*)ref;

            unsigned short id = sref[0];
            if (id == BLOCK_AIR || id == INVALID_BLOCK_ID) {
                // TraceLog(LOG_INFO, "Invalid block id %d at pos %d(%d)", (int)ref, i);
                continue;
            }

            short x = sref[1];
            short y = sref[2];
            unsigned short gid = sref[3];

            uint8_t bitdata = ref[9];

            struct fblock block = _fBlockFromId(id);

            block.layer_id = 0;
            block.base.block_x = x;
            block.base.block_y = y;
            if (parse_bitfield) _fBlockRecoverBitfield(&block, bitdata);

            struct flevel_registry_entry entry = { 0 };
            entry.id = rid;
            entry.entry = nbt_new_tag_compound();
            nbt_tag_t *group_id_tag = nbt_new_tag_short(gid);
            nbt_set_tag_name_easy(group_id_tag, "gid");
            nbt_tag_compound_append(entry.entry, group_id_tag);

            block.linked_reg = RSBAddElement_lre(level->block_entries, entry);
            level->objects[i] = block;
            level->last_entry_id = rid;

            rid++;
        }

        break;
    }
    case 3: {
        TraceLog(LOG_INFO, "Loading outdated level (3)");

        unsigned short width = ((short*)data)[1];
        unsigned short height = ((short*)data)[2];

        unsigned char parse_bitfield = 0;

        TraceLog(LOG_INFO, "Level size: %dx%d", width, height);

        if (width == 0xFFFF) {
            parse_bitfield = 1;
            TraceLog(LOG_INFO, "Parsing bitfields for outdated level");
        }

        unsigned int objects = *(unsigned int*)(data + 6) % 3000000;

        TraceLog(LOG_INFO, "Objects: %d", objects);

        level = _fLevelLoadTest(__state.tilemap, (IVector2) { 28, 4 });;

        free(level->objects);

        level->width = width;
        level->height = height;
        level->data_size = objects;
        level->objects = (struct fblock*)MemAlloc(sizeof(struct fblock) * level->data_size);

        unsigned int rid = 1;

        TraceLog(LOG_INFO, "Analyzing level layers");

        rsb_array__int *layer_array = RSBCreateArray_int();

        for (unsigned int i = 0; i < objects; i++) {
            unsigned int offset = 10 + (11 * i);
            if (offset >= len) {
                TraceLog(LOG_WARNING, "Unexpected EOF at %d", offset);
                break;
            }

            unsigned char* ref = data + offset;
            short* sref = (short*)ref;

            unsigned short id = sref[0];
            if (id == BLOCK_AIR || id == INVALID_BLOCK_ID) {
                // TraceLog(LOG_INFO, "Invalid block id %d at pos %d(%d)", (int)ref, i);
                continue;
            }

            short x = sref[1];
            short y = sref[2];
            unsigned short gid = sref[3];
            unsigned short lid = sref[4];

            uint8_t bitdata = ref[11];

            if (!RSBContains_int(layer_array, lid)) {
                RSBAddElement_int(layer_array, lid);
            }
        }

        if (layer_array->len > 0) {
            TraceLog(LOG_INFO, "Level contains %d layers; sorting block data", layer_array->len);
        } else {
            TraceLog(LOG_WARNING, "Cannot determine layer metadata; parsing blocks in classical way");

            RSBAddElement_int(layer_array, 0);
        }

        for (unsigned int j = 0; j < layer_array->len; j++) {
            unsigned short wanted_layer = layer_array->objects[j];
            TraceLog(LOG_ERROR, "Processing layer %d", wanted_layer);
            for (unsigned int i = 0; i < objects; i++) {
                unsigned int offset = 10 + (11 * i);
                if (offset >= len) {
                    TraceLog(LOG_WARNING, "Unexpected EOF at %d", offset);
                    break;
                }

                unsigned char* ref = data + offset;
                short* sref = (short*)ref;

                unsigned short id = sref[0];
                if (id == BLOCK_AIR || id == INVALID_BLOCK_ID) {
                    // TraceLog(LOG_INFO, "Invalid block id %d at pos %d(%d)", (int)ref, i);
                    continue;
                }

                short x = sref[1];
                short y = sref[2];
                unsigned short gid = sref[3];
                unsigned short lid = sref[4];

                if (lid != wanted_layer) continue;

                uint8_t bitdata = ref[11];

                if (!RSBContains_int(layer_array, lid)) {
                    RSBAddElement_int(layer_array, lid);
                }

                struct fblock block = _fBlockFromId(id);
                block.layer_id = lid;
                block.base.block_x = x;
                block.base.block_y = y;
                if (parse_bitfield) _fBlockRecoverBitfield(&block, bitdata);

                struct flevel_registry_entry entry = { 0 };
                entry.id = rid;
                // TraceLog(LOG_INFO, "Creating unitype for %d", rid);
                entry.entry = nbt_new_tag_compound();
                nbt_tag_t *group_id_tag = nbt_new_tag_short(gid);
                nbt_set_tag_name_easy(group_id_tag, "gid");
                nbt_tag_compound_append(entry.entry, group_id_tag);

                block.linked_reg = RSBAddElement_lre(level->block_entries, entry);
                level->objects[i] = block;
                level->last_entry_id = rid;

                RSBAddElement_lre(level->block_entries, entry);

                rid++;
            }
        }

        RSBDestroy_int(layer_array);

        break;
    }
    case 4:
    case LEVEL_FORMAT_VERSION: {
        TraceLog(LOG_INFO, "Reopening level");
        MemFree(data);
        data = NULL;

        FILE* file = fopen(filename, "rb");
        if (!file) {
            TraceLog(LOG_INFO, "Could not reopen level");
            break;
        }

        unsigned short lfv = 0;
        unsigned short width = 0;
        unsigned short height = 0;
        unsigned int objects = 0;
        unsigned char endianness = 0;

        fread(&lfv, sizeof(lfv), 1, file);
        fread(&width, sizeof(width), 1, file);
        fread(&height, sizeof(height), 1, file);
        fread(&objects, sizeof(objects), 1, file);
        fread(&endianness, sizeof(endianness), 1, file);

        unsigned char ie = 0;
#ifdef TARGET_BIG_ENDIAN
        ie = endianess == 1;
#else
        ie = endianness == 0;
#endif

        if (!ie) {
            fclose(file);
            TraceLog(LOG_ERROR, "Level has been saved on a system with different byte order");

            return NULL;
        }

        TraceLog(LOG_INFO, "Level size: %dx%d", width, height);
        TraceLog(LOG_INFO, "Objects: %d", objects);

        level = _fLevelLoadTest(__state.tilemap, (IVector2) { 28, 4 });

        free(level->objects);

        level->width = width;
        level->height = height;
        level->data_size = objects;
        level->objects = (struct fblock*)MemAlloc(sizeof(struct fblock) * level->data_size);

        unsigned char* block_entries_ptr = NULL;

        TraceLog(LOG_INFO, "Analyzing level layers");

        long cur_pos = ftell(file);
        rsb_array__int *layer_array = RSBCreateArray_int();

        for (unsigned int i = 0; i < objects; i++) {
            unsigned int registry_id = 0;
            unsigned short id = 0, layer_id = 0;
            short x = 0, y = 0;
            unsigned char bitdata = 0;

            fread(&id, sizeof(id), 1, file);
            fread(&x, sizeof(x), 1, file);
            fread(&y, sizeof(y), 1, file);
            fread(&layer_id, sizeof(layer_id), 1, file);
            fread(&registry_id, sizeof(registry_id), 1, file);
            fread(&bitdata, sizeof(bitdata), 1, file);

            if (!RSBContains_int(layer_array, layer_id)) {
                RSBAddElement_int(layer_array, layer_id);
            }

            // if (id == BLOCK_AIR || id == INVALID_BLOCK_ID) {
            //     // TraceLog(LOG_INFO, "Invalid block id %d at pos %d(%d)", (int)ref, i);
            //     continue;
            // }

            // struct fblock block = _fBlockFromId(id);
            // block.layer_id = layer_id;
            // block.registry_id = registry_id;
            // block.base.block_x = x;
            // block.base.block_y = y;
            // _fBlockRecoverBitfield(&block, bitdata);

            // level->objects[i] = block;
        }

        if (layer_array->len > 0) {
            TraceLog(LOG_INFO, "Level contains %d layers; sorting block data", layer_array->len);
        } else {
            TraceLog(LOG_WARNING, "Cannot determine layer metadata; parsing blocks in classical way");

            RSBAddElement_int(layer_array, 0);
        }

        for (unsigned int j = 0; j < layer_array->len; j++) {
            fseek(file, cur_pos, SEEK_SET);

            unsigned short wanted_layer = layer_array->objects[j];
            TraceLog(LOG_ERROR, "Processing layer %d", wanted_layer);

            for (unsigned int i = 0; i < objects; i++) {
                unsigned int registry_id = 0;
                unsigned short id = 0, layer_id = 0;
                short x = 0, y = 0;
                unsigned char bitdata = 0;

                fread(&id, sizeof(id), 1, file);
                fread(&x, sizeof(x), 1, file);
                fread(&y, sizeof(y), 1, file);
                fread(&layer_id, sizeof(layer_id), 1, file);
                fread(&registry_id, sizeof(registry_id), 1, file);
                fread(&bitdata, sizeof(bitdata), 1, file);

                if (layer_id != wanted_layer) continue;

                if (!RSBContains_int(layer_array, layer_id)) {
                    RSBAddElement_int(layer_array, layer_id);
                }

                if (id == BLOCK_AIR || id == INVALID_BLOCK_ID) {
                    // TraceLog(LOG_INFO, "Invalid block id %d at pos %d(%d)", (int)ref, i);
                    continue;
                }

                struct fblock block = _fBlockFromId(id);
                block.layer_id = layer_id;
                block.registry_id = registry_id;
                block.base.block_x = x;
                block.base.block_y = y;
                _fBlockRecoverBitfield(&block, bitdata);

                level->objects[i] = block;
            }
        }

        unsigned int be_amount;
        fread(&be_amount, sizeof(be_amount), 1, file);

        TraceLog(LOG_INFO, "Amount of saved block entries: %d", be_amount);

        if (format == LEVEL_FORMAT_VERSION) {
            TraceLog(LOG_INFO, "Using block entries v5 implementation");
            for (unsigned int i = 0; i < be_amount; i++) {
                unsigned int id;
                fread(&id, sizeof(id), 1, file);

                unsigned int bytes;
                fread(&bytes, sizeof(bytes), 1, file);

                void *nbt_v = MemAlloc(bytes);
                fread(nbt_v, bytes, 1, file);

                nbt__read_stream_t stream = {};
                stream.buffer = nbt_v;
                stream.buffer_offset = 0;
                nbt_tag_t *root = nbt__parse(&stream, 1, NBT_NO_OVERRIDE);

                MemFree(nbt_v);

                if (root == NULL) {
                    TraceLog(LOG_INFO, "Could NOT parse NBT data of streamed block (%ld bytes; id=%ld)", bytes, id);
                } else {
                    TraceLog(LOG_INFO, "Tag type (%ld): %d (bytes=%ld)", id, (int)root->type, bytes);

                    print_nbt_tree(root, 2);

                    struct flevel_registry_entry entry = { 0 };
                    entry.id = id;
                    entry.entry = root;

                    struct fblock *reg = _fLevelBlockFromRegistry(level, entry.id);
                    if (!reg) {
                        TraceLog(LOG_WARNING, "Unused registry entry %d was found", entry.id);
                    } else {
                        reg->linked_reg = RSBAddElement_lre(level->block_entries, entry);
                    }
                }
            }
        } else if (format == 4) {
            TraceLog(LOG_WARNING, "Block entries v4 are not supported yet");
        }

        // for (unsigned int i = 0; i < be_amount; i++) {
        //     unsigned int id;
        //     fread(&id, sizeof(id), 1, file);

        //     unsigned int extra_objects_len;
        //     fread(&extra_objects_len, sizeof(extra_objects_len), 1, file);

        //     level->last_entry_id = id;

        //     // TraceLog(LOG_INFO, "Creating unitype root for %d (%d)", id, extra_objects_len);

        //     unitype_t* root = __uni_create(NULL);

        //     struct flevel_registry_entry entry = { 0 };
        //     entry.id = id;
        //     entry.entry = root;

        //     for (unsigned int j = 0; j < extra_objects_len; j++) {
        //         size_t name_len;
        //         fread(&name_len, sizeof(name_len), 1, file);

        //         char* name = (char *)malloc(name_len + 1);
        //         if (name_len != 0) fread(name, sizeof(char), name_len, file);
        //         name[name_len] = '\0';

        //         char type;
        //         fread(&type, sizeof(type), 1, file);

        //         unsigned int p_sz;
        //         fread(&p_sz, sizeof(p_sz), 1, file);

        //         void* p = NULL;
        //         if (p_sz != 0) {
        //             p = malloc(p_sz);
        //             fread(p, 1, p_sz, file);
        //         }

        //         unitype_t* u = __uni_createbase(name, type);
        //         if (u->p) free(u->p);
        //         u->p = p;
        //         u->p_sz = p_sz;

        //         __uni_add(root, u);
        //     }

        //     struct fblock *reg = _fLevelBlockFromRegistry(level, entry.id);
        //     if (!reg) {
        //         // TraceLog(LOG_WARNING, "Unused registry entry %d was found", entry.id);
        //     } else {
        //         reg->linked_reg = RSBAddElement_lre(level->block_entries, entry);
        //     }
        // }

        fclose(file);

        break;
    }
    default: {
        TraceLog(LOG_ERROR, "Unknown level format version");
        break;
    }
    }

    if (data != NULL) MemFree(data);
    if (level) level->level_source = LEVEL_SOURCE_FILE;

    return level;
}

struct flevel* _fLevelLoadFromFileSelector(const char* filename) {
    struct flevel *ret = NULL;

    if (!filename) {
        TraceLog(LOG_INFO, "User aborted file selection");

        if (__state.current_level != NULL) {
            TraceLog(LOG_INFO, "Opening already loaded level (%p)", __state.current_level);
            ret = __state.current_level;
        } else {
            TraceLog(LOG_INFO, "Opening template level");
            ret = _fLevelLoadTest(__state.tilemap, (IVector2){ 28, 4 });
        }
    } else {
        TraceLog(LOG_INFO, "Opening chosen level");
        struct flevel *ref = _fLevelLoadFromFile(filename);
        if (!ref) {
            TraceLog(LOG_INFO, "Could not open level. Opening template level");
            _fNotifMgrSend(_fTranslationGetString("notification.level.load_failed"));
            ret = _fLevelLoadTest(__state.tilemap, (IVector2){ 28, 4 });
        }
        else {
            TraceLog(LOG_INFO, "Opening chosen level");

            ret = ref;
        }
    }

    if (!ret) {
        TraceLog(LOG_INFO, "Could not open level because of unknown reasons");
        _fNotifMgrSend(_fTranslationGetString("notification.level.load_failed"));
    } else {
        ret->hitboxes = _fLevelGetHitboxes(ret);
    }

    return ret;
}
