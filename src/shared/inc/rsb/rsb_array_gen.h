#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 1)

#include <assert.h>

#define RSB_ARRAY_NAME(funname) rsb_array_##funname

#define RSB_ARRAY_STRUCT(type, funname) typedef struct RSB_ARRAY_NAME(funname) { \
    unsigned int len;           \
    unsigned int current_index; \
    unsigned int added_elements;\
    type *objects;              \
    unsigned char lock;         \
} RSB_ARRAY_NAME(funname);

#define RSB_ARRAY_FUNC_CREATE_DEF(funname) RSB_ARRAY_NAME(funname) *RSBCreateArray##funname()
#define RSB_ARRAY_FUNC_CREATEFL_DEF(type, funname) RSB_ARRAY_NAME(funname) *RSBCreateArrayFromList##funname(type *object_list, unsigned int size)
#define RSB_ARRAY_FUNC_ADDELEMENT_DEF(type, funname) void RSBAddElement##funname(RSB_ARRAY_NAME(funname) *array, type object)
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
    array->len = 0;                                                                                         \
    array->current_index = 0;                                                                               \
    array->added_elements = 0;                                                                              \
    array->objects = NULL;                                                                                  \
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
    if (!array) return;                                                                                                     \
    while (array->lock) {}                                                                                                  \
    array->lock = 1;                                                                                                        \
                                                                                                                            \
    if (!array->objects) {                                                                                                  \
        while (!array->objects) array->objects = (type *)calloc(1, sizeof(type));                                           \
        array->len = 1;                                                                                                     \
        array->current_index = 0;                                                                                           \
        array->added_elements = 0;                                                                                          \
    }                                                                                                                       \
                                                                                                                            \
    if ((array->added_elements + 1) > array->len) {                                                                         \
        array->objects = (type *)realloc(array->objects, sizeof(type) * (array->len + 1));                                  \
        while (!array->objects) array->objects = (type *)calloc(array->len + 1, sizeof(type));                               \
        array->len++;                                                                                                       \
    }                                                                                                                       \
                                                                                                                            \
    array->objects[array->current_index] = object;                                                                          \
    array->added_elements++;                                                                                                \
    array->current_index++;                                                                                                 \
                                                                                                                            \
    array->lock = 0;                                                                                                        \
                                                                                                                            \
    return;                                                                                                                 \
}

#define RSB_ARRAY_FUNC_POPELEMENT_IMPL(type, funname) RSB_ARRAY_FUNC_POPELEMENT_DEF(funname) {                          \
    if (!array || !array->objects || array->added_elements == 0 || array->len == 0) return;                             \
    while (array->lock) {}                                                                                              \
    array->lock = 1;                                                                                                    \
    if ((array->len - 1) == 0) {                                                                                        \
        free(array->objects);                                                                                           \
        array->objects = NULL;                                                                                          \
                                                                                                                        \
        array->len--;                                                                                                   \
        array->added_elements--;                                                                                        \
                                                                                                                        \
        return;                                                                                                         \
    }                                                                                                                   \
                                                                                                                        \
    array->objects = (type *)realloc(array->objects, sizeof(type) * (array->len - 1));                                  \
                                                                                                                        \
    array->len--;                                                                                                       \
    array->added_elements--;                                                                                            \
                                                                                                                        \
    if (array->current_index > array->added_elements) {                                                                 \
        if (array->current_index != 0) array->current_index--;                                                          \
    }                                                                                                                   \
                                                                                                                        \
    array->lock = 0;                                                                                                    \
                                                                                                                        \
    return;                                                                                                             \
}

#define RSB_ARRAY_FUNC_DESTROY_IMPL(funname) RSB_ARRAY_FUNC_DESTROY_DEF(funname) {    \
    if (!array || !array->objects) return;                                            \
                                                                                      \
    free(array->objects);                                                             \
    free(array);                                                                      \
                                                                                      \
    return;                                                                           \
}

#define RSB_ARRAY_FUNC_GETATINDEX_IMPL(type, funname) RSB_ARRAY_FUNC_GETATINDEX_DEF(type, funname) {  \
    assert(!(!array || !array->objects || array->len < index));                                       \
    while (array->lock) {}                                                                            \
                                                                                                      \
    return array->objects[index];                                                                     \
}

#define RSB_ARRAY_FUNC_MERGE_IMPL(type, funname) RSB_ARRAY_FUNC_MERGE_DEF(type, funname) {  \
    if (!source || !destination || !source->objects || !destination->objects) return;       \
    while (destination->lock) {}                                                            \
    destination->lock = 1;                                                                  \
                                                                                            \
    for (size_t i = 0; i < source->added_elements; i++) {                                   \
        type obj = RSBGetAtIndex##funname(source, i);                                       \
                                                                                            \
        RSBAddElement##funname(destination, obj);                                           \
    }                                                                                       \
                                                                                            \
    destination->lock = 0;                                                                  \
    return;                                                                                 \
}

#define RSB_ARRAY_FUNC_ADDELEMENTATINDEX_IMPL(type, funname) RSB_ARRAY_FUNC_ADDELEMENTATINDEX_DEF(type, funname) {  \
    if (!array || index > array->len) return;                                                                       \
    while (array->lock) {}                                                                                          \
    array->lock = 0;                                                                                                \
                                                                                                                    \
    rsb_array_##funname *new_arr = RSBCreateArray##funname();                                                       \
                                                                                                                    \
    for (size_t i = 0; i < array->added_elements; i++) {                                                            \
        if (i == index) {                                                                                           \
            RSBAddElement##funname(new_arr, object);                                                                \
        }                                                                                                           \
                                                                                                                    \
        RSBAddElement##funname(new_arr, RSBGetAtIndex##funname(array, i));                                          \
    }                                                                                                               \
                                                                                                                    \
    RSBClear##funname(array);                                                                                       \
                                                                                                                    \
    for (size_t i = 0; i < new_arr->added_elements; i++) {                                                          \
        RSBAddElement##funname(array, RSBGetAtIndex##funname(new_arr, i));                                          \
    }                                                                                                               \
                                                                                                                    \
    RSBDestroy##funname(new_arr);                                                                                   \
    array->lock = 0;                                                                                                \
}

#define RSB_ARRAY_FUNC_POPELEMENTATINDEX_IMPL(type, funname) RSB_ARRAY_FUNC_POPELEMENTATINDEX_DEF(type, funname) {  \
    if (!array || index > array->len) return;                                                                       \
    while (array->lock) {}                                                                                          \
    array->lock = 0;                                                                                                \
                                                                                                                    \
    rsb_array_##funname *new_arr = RSBCreateArray##funname();                                                       \
                                                                                                                    \
    for (size_t i = 0; i < array->added_elements; i++) {                                                            \
        if (i != index) {                                                                                           \
            RSBAddElement##funname(new_arr, RSBGetAtIndex##funname(array, i));                                      \
        }                                                                                                           \
    }                                                                                                               \
                                                                                                                    \
    RSBClear##funname(array);                                                                                       \
                                                                                                                    \
    for (size_t i = 0; i < new_arr->added_elements; i++) {                                                          \
        RSBAddElement##funname(array, RSBGetAtIndex##funname(new_arr, i));                                          \
    }                                                                                                               \
                                                                                                                    \
    RSBDestroy##funname(new_arr);                                                                                   \
    array->lock = 0;                                                                                                \
}

#define RSB_ARRAY_FUNC_CLEAR_IMPL(type, funname) RSB_ARRAY_FUNC_CLEAR_DEF(type, funname) {  \
    if (!array || !array->objects || !array->len) return;                                   \
    while (array->lock) {}                                                                  \
    array->lock = 1;                                                                        \
                                                                                            \
    free(array->objects);                                                                   \
                                                                                            \
    array->len = 0;                                                                         \
    array->current_index = 0;                                                               \
    array->added_elements = 0;                                                              \
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

#pragma pack(pop)

#ifdef __cplusplus
}
#endif
