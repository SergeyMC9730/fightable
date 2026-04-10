#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <assert.h>
#include <fightable/pthread_compat.h>
#include <stdio.h>

#define RSB_ARRAY_NAME(funname) rsb_array_##funname

#define RSB_ARRAY_STRUCT(type, funname) typedef struct RSB_ARRAY_NAME(funname) { \
    unsigned int alloc_len;                                                                                 \
    unsigned int current_index;                                                                             \
    unsigned int len;                                                                                       \
    type *objects;                                                                                          \
    unsigned char lock;                                                                                     \
    void (*callback_address_change)(struct RSB_ARRAY_NAME(funname) *ctx, void *old_addr, void *new_addr);   \
} RSB_ARRAY_NAME(funname);

#define RSB_ARRAY_FUNC_CREATE_DEF(funname) RSB_ARRAY_NAME(funname) *RSBCreateArray##funname()
#define RSB_ARRAY_FUNC_CREATEFL_DEF(type, funname) RSB_ARRAY_NAME(funname) *RSBCreateArrayFromList##funname(type *object_list, unsigned int size)
#define RSB_ARRAY_FUNC_ADDELEMENT_DEF(type, funname) type *RSBAddElement##funname(RSB_ARRAY_NAME(funname) *array, type object)
#define RSB_ARRAY_FUNC_POPELEMENT_DEF(funname) void RSBPopElement##funname(RSB_ARRAY_NAME(funname) *array)
#define RSB_ARRAY_FUNC_DESTROY_DEF(funname) void RSBDestroy##funname(RSB_ARRAY_NAME(funname) *array)
#define RSB_ARRAY_FUNC_GETATINDEX_DEF(type, funname) type RSBGetAtIndex##funname(RSB_ARRAY_NAME(funname) *array, unsigned int index)
#define RSB_ARRAY_FUNC_MERGE_DEF(type, funname) void RSBMergeElements##funname(RSB_ARRAY_NAME(funname) *source, RSB_ARRAY_NAME(funname) *destination)
#define RSB_ARRAY_FUNC_ADDELEMENTATINDEX_DEF(type, funname) void RSBAddElementAtIndex##funname(RSB_ARRAY_NAME(funname) *array, type object, unsigned int index)
#define RSB_ARRAY_FUNC_POPELEMENTATINDEX_DEF(type, funname) void RSBPopElementAtIndex##funname(RSB_ARRAY_NAME(funname) *array, unsigned int index)
#define RSB_ARRAY_FUNC_CLEAR_DEF(type, funname) void RSBClear##funname(RSB_ARRAY_NAME(funname) *array)
#define RSB_ARRAY_FUNC_VALID_DEF(type, funname) unsigned char RSBIsValid##funname(RSB_ARRAY_NAME(funname) *array)

#define RSB_ARRAY_DEF_GEN(type, funname) RSB_ARRAY_STRUCT(type, funname); \
RSB_ARRAY_FUNC_CREATE_DEF(funname);                 \
RSB_ARRAY_FUNC_CREATEFL_DEF(type, funname);         \
RSB_ARRAY_FUNC_DESTROY_DEF(funname);                \
                                                    \
RSB_ARRAY_FUNC_ADDELEMENT_DEF(type, funname);       \
RSB_ARRAY_FUNC_POPELEMENT_DEF(funname);             \
RSB_ARRAY_FUNC_MERGE_DEF(type, funname);            \
                                                    \
RSB_ARRAY_FUNC_GETATINDEX_DEF(type, funname);       \
                                                    \
RSB_ARRAY_FUNC_ADDELEMENTATINDEX_DEF(type, funname);\
RSB_ARRAY_FUNC_POPELEMENTATINDEX_DEF(type, funname);\
                                                    \
RSB_ARRAY_FUNC_CLEAR_DEF(type, funname);            \
                                                    \
RSB_ARRAY_FUNC_VALID_DEF(type, funname);

#include <stdlib.h>

#define RSB_ARRAY_FUNC_CREATE_IMPL(funname) RSB_ARRAY_FUNC_CREATE_DEF(funname) {                            \
    RSB_ARRAY_NAME(funname) *array = (RSB_ARRAY_NAME(funname) *)calloc(1, sizeof(RSB_ARRAY_NAME(funname))); \
                                                                                                            \
    array->alloc_len = 0;                                                                                   \
    array->current_index = 0;                                                                               \
    array->len = 0;                                                                                         \
    array->objects = NULL;                                                                                  \
    array->lock = 0;                                                                                        \
                                                                                                            \
    return array;                                                                                           \
}

#define RSB_ARRAY_FUNC_CREATEFL_IMPL(type, funname) RSB_ARRAY_FUNC_CREATEFL_DEF(type, funname) { \
    RSB_ARRAY_NAME(funname) *array = RSBCreateArray##funname(); \
                                                                \
    if (object_list == NULL) return array;                      \
                                                                \
    for (unsigned int i = 0; i < size; i++) {                   \
        RSBAddElement##funname(array, object_list[i]);          \
    }                                                           \
                                                                \
    return array;                                               \
}

#define RSB_ARRAY_FUNC_ADDELEMENT_IMPL(type, funname) RSB_ARRAY_FUNC_ADDELEMENT_DEF(type, funname) {                        \
    if (!array) return NULL;                                                                                                \
    while (array->lock) {}                                                                                                  \
    array->lock = 1;                                                                                                        \
                                                                                                                            \
    if (!array->objects) {                                                                                                  \
        array->alloc_len = 8;                                                                                               \
        array->objects = (type *)calloc(array->alloc_len, sizeof(type));                                                    \
        array->current_index = 0;                                                                                           \
        array->len = 0;                                                                                                     \
    }                                                                                                                       \
                                                                                                                            \
    unsigned char ra = 0;                                                                                                   \
    while ((array->len + 1) >= array->alloc_len) { array->alloc_len += 8; ra = 1; }                                         \
    if (ra) {                                                                                                               \
        void *old = array->objects;                                                                                         \
        printf("realloc required for %s: alloc_len=%d, objects=%p; ", #type, array->alloc_len - 8, array->objects);         \
        array->objects = (type *)reallocarray(array->objects, array->alloc_len, sizeof(type));                              \
        printf("new_ptr=%p; alloc_len=%d; new_len=%d\n", array->objects, array->alloc_len, array->len + 1);                 \
        if (old != array->objects && array->callback_address_change) {                                                      \
            array->callback_address_change(array, old, array->objects);                                                     \
        }                                                                                                                   \
    }                                                                                                                       \
                                                                                                                            \
    array->objects[array->current_index] = object;                                                                          \
    array->len++;                                                                                                           \
    array->current_index++;                                                                                                 \
                                                                                                                            \
    array->lock = 0;                                                                                                        \
                                                                                                                            \
    return array->objects + array->current_index - 1;                                                                       \
}

#define RSB_ARRAY_FUNC_POPELEMENT_IMPL(type, funname) RSB_ARRAY_FUNC_POPELEMENT_DEF(funname) {                          \
    if (!array || !array->objects || array->len == 0 || array->alloc_len == 0) return;                                  \
    while (array->lock) {}                                                                                              \
    array->lock = 1;                                                                                                    \
    if ((array->alloc_len - 1) == 0) {                                                                                  \
        free(array->objects);                                                                                           \
        array->objects = NULL;                                                                                          \
                                                                                                                        \
        array->len = 0;                                                                                                 \
        array->alloc_len = 0;                                                                                           \
        array->lock = 0;                                                                                                \
                                                                                                                        \
        return;                                                                                                         \
    }                                                                                                                   \
                                                                                                                        \
    /*array->objects = (type *)realloc(array->objects, sizeof(type) * (array->len - 1));*/                              \
                                                                                                                        \
    array->alloc_len--;                                                                                                 \
    array->len--;                                                                                                       \
    array->current_index--;                                                                                             \
                                                                                                                        \
    array->lock = 0;                                                                                                    \
                                                                                                                        \
    return;                                                                                                             \
}

#define RSB_ARRAY_FUNC_DESTROY_IMPL(funname) RSB_ARRAY_FUNC_DESTROY_DEF(funname) {    \
    if (!array) return;                                                               \
                                                                                      \
    if (array->objects) free(array->objects);                                         \
    free(array);                                                                      \
                                                                                      \
    return;                                                                           \
}

#define RSB_ARRAY_FUNC_GETATINDEX_IMPL(type, funname) RSB_ARRAY_FUNC_GETATINDEX_DEF(type, funname) {  \
    assert(array);                                                                                    \
    if (!array->objects || array->len <= index) return (type){};                                      \
    while (array->lock) {}                                                                            \
                                                                                                      \
    return array->objects[index];                                                                     \
}

#define RSB_ARRAY_FUNC_MERGE_IMPL(type, funname) RSB_ARRAY_FUNC_MERGE_DEF(type, funname) {  \
    if (!source || !destination || !source->objects || !destination->objects) return;       \
    while (destination->lock) {}                                                            \
    destination->lock = 0;                                                                  \
                                                                                            \
    for (size_t i = 0; i < source->len; i++) {                                              \
        type obj = RSBGetAtIndex##funname(source, i);                                       \
                                                                                            \
        RSBAddElement##funname(destination, obj);                                           \
    }                                                                                       \
                                                                                            \
    destination->lock = 0;                                                                  \
    return;                                                                                 \
}

#define RSB_ARRAY_FUNC_ADDELEMENTATINDEX_IMPL(type, funname) RSB_ARRAY_FUNC_ADDELEMENTATINDEX_DEF(type, funname) {  \
    if (!array || index > array->alloc_len) return;                                                                 \
    while (array->lock) {}                                                                                          \
    array->lock = 1;                                                                                                \
                                                                                                                    \
    rsb_array_##funname *new_arr = RSBCreateArray##funname();                                                       \
                                                                                                                    \
    for (size_t i = 0; i < array->len; i++) {                                                                       \
        if (i == index) {                                                                                           \
            new_arr->lock = 0;                                                                                      \
            RSBAddElement##funname(new_arr, object);                                                                \
        }                                                                                                           \
        array->lock = 0;                                                                                            \
        new_arr->lock = 0;                                                                                          \
        RSBAddElement##funname(new_arr, RSBGetAtIndex##funname(array, i));                                          \
        array->lock = 1;                                                                                            \
    }                                                                                                               \
                                                                                                                    \
    array->lock = 0;                                                                                                \
    RSBClear##funname(array);                                                                                       \
                                                                                                                    \
    for (size_t i = 0; i < new_arr->len; i++) {                                                                     \
        RSBAddElement##funname(array, RSBGetAtIndex##funname(new_arr, i));                                          \
    }                                                                                                               \
                                                                                                                    \
    RSBDestroy##funname(new_arr);                                                                                   \
    array->lock = 0;                                                                                                \
}

#define RSB_ARRAY_FUNC_POPELEMENTATINDEX_IMPL(type, funname) RSB_ARRAY_FUNC_POPELEMENTATINDEX_DEF(type, funname) {  \
    if (!array || index > array->alloc_len) return;                                                                 \
    while (array->lock) {}                                                                                          \
    array->lock = 1;                                                                                                \
                                                                                                                    \
    rsb_array_##funname *new_arr = RSBCreateArray##funname();                                                       \
                                                                                                                    \
    for (size_t i = 0; i < array->len; i++) {                                                                       \
        if (i != index) {                                                                                           \
            array->lock = 0;                                                                                        \
            new_arr->lock = 0;                                                                                      \
            RSBAddElement##funname(new_arr, RSBGetAtIndex##funname(array, i));                                      \
            array->lock = 1;                                                                                        \
        }                                                                                                           \
    }                                                                                                               \
                                                                                                                    \
    array->lock = 0;                                                                                                \
    RSBClear##funname(array);                                                                                       \
                                                                                                                    \
    for (size_t i = 0; i < new_arr->len; i++) {                                                                     \
        new_arr->lock = 0;                                                                                          \
        array->lock = 0;                                                                                            \
        RSBAddElement##funname(array, RSBGetAtIndex##funname(new_arr, i));                                          \
    }                                                                                                               \
                                                                                                                    \
    RSBDestroy##funname(new_arr);                                                                                   \
    array->lock = 0;                                                                                                \
}

#define RSB_ARRAY_FUNC_CLEAR_IMPL(type, funname) RSB_ARRAY_FUNC_CLEAR_DEF(type, funname) {  \
    if (!array || !array->objects || !array->alloc_len) return;                             \
    while (array->lock) {}                                                                  \
    array->lock = 1;                                                                        \
                                                                                            \
    free(array->objects);                                                                   \
                                                                                            \
    array->len = 0;                                                                         \
    array->current_index = 0;                                                               \
    array->alloc_len = 0;                                                                   \
    array->objects = 0;                                                                     \
    array->lock = 0;                                                                        \
}

#define RSB_ARRAY_FUNC_VALID_IMPL(type, funname) RSB_ARRAY_FUNC_VALID_DEF(type, funname) {  \
    return array != NULL && array->objects != NULL;                                         \
}

#define RSB_ARRAY_IMPL_GEN(type, funname)               \
RSB_ARRAY_FUNC_CREATE_IMPL(funname);                    \
RSB_ARRAY_FUNC_CREATEFL_IMPL(type, funname);            \
RSB_ARRAY_FUNC_DESTROY_IMPL(funname);                   \
                                                        \
RSB_ARRAY_FUNC_ADDELEMENT_IMPL(type, funname);          \
RSB_ARRAY_FUNC_POPELEMENT_IMPL(type, funname);          \
RSB_ARRAY_FUNC_MERGE_IMPL(type, funname);               \
                                                        \
RSB_ARRAY_FUNC_GETATINDEX_IMPL(type, funname);          \
                                                        \
RSB_ARRAY_FUNC_ADDELEMENTATINDEX_IMPL(type, funname);   \
RSB_ARRAY_FUNC_POPELEMENTATINDEX_IMPL(type, funname);   \
                                                        \
RSB_ARRAY_FUNC_CLEAR_IMPL(type, funname);               \
                                                        \
RSB_ARRAY_FUNC_VALID_IMPL(type, funname);

#ifdef __cplusplus
}
#endif
