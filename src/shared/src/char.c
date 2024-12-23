#include <rsb/rsb_array_char.h>

rsb_array__char* RSBCreateArray_char() {
    rsb_array__char* array = (rsb_array__char*)calloc(1, sizeof(rsb_array__char)); array->len = 0; array->current_index = 0; array->added_elements = 0; array->objects = ((void*)0); return array;
}; rsb_array__char* RSBCreateArrayFromList_char(char* object_list, unsigned int size) {
    rsb_array__char* array = RSBCreateArray_char(); if (object_list == ((void*)0)) return array; for (unsigned int i = 0; i < size; i++) {
        RSBAddElement_char(array, object_list[i]);
    } return array;
}; void RSBDestroy_char(rsb_array__char* array) {
    if (!array || !array->objects) return; free(array->objects); free(array); return;
}; void RSBAddElement_char(rsb_array__char* array, char object) {
    if (!array) return; while (array->lock) {
    } array->lock = 1; if (!array->objects) {
        array->objects = (char*)calloc(1, sizeof(char)); array->len = 1; array->current_index = 0; array->added_elements = 0;
    } if ((array->added_elements + 1) > array->len) {
        array->objects = (char*)realloc(array->objects, sizeof(char) * (array->len + 1)); array->len++;
    } array->objects[array->current_index] = object; array->added_elements++; array->current_index++; array->lock = 0; return;
}; void RSBPopElement_char(rsb_array__char* array) {
    if (!array || !array->objects || array->added_elements == 0 || array->len == 0) return; while (array->lock) {
    } array->lock = 1; if ((array->len - 1) == 0) {
        free(array->objects); array->objects = ((void*)0); array->len--; array->added_elements--; return;
    } array->objects = (char*)realloc(array->objects, sizeof(char) * (array->len - 1)); array->len--; array->added_elements--; if (array->current_index > array->len) {
        if (array->current_index != 0) array->current_index--;
    } array->lock = 0; return;
}; void RSBMergeElements_char(rsb_array__char* source, rsb_array__char* destination) {
    if (!source || !destination || !source->objects || !destination->objects) return; while (destination->lock) {
    } destination->lock = 1; for (size_t i = 0; i < source->len; i++) {
        char obj = RSBGetAtIndex_char(source, i); RSBAddElement_char(destination, obj);
    } destination->lock = 0; return;
}; char RSBGetAtIndex_char(rsb_array__char* array, unsigned int index) {
    ((void)0); while (array->lock) {
    } return array->objects[index];
}; void RSBAddElementAtIndex_char(rsb_array__char* array, char object, unsigned int index) {
    if (!array || index > array->len) return; while (array->lock) {
    } array->lock = 1; rsb_array__char* new_arr = RSBCreateArray_char(); for (size_t i = 0; i < array->len; i++) {
        if (i == index) {
            RSBAddElement_char(new_arr, object);
        } RSBAddElement_char(new_arr, RSBGetAtIndex_char(array, i));
    } while (array->len != 0) {
        RSBPopElement_char(array);
    } for (size_t i = 0; i < new_arr->len; i++) {
        RSBAddElement_char(array, RSBGetAtIndex_char(new_arr, i));
    } RSBDestroy_char(new_arr); array->lock = 0;
}; void RSBPopElementAtIndex_char(rsb_array__char* array, unsigned int index) {
    if (!array || index > array->len) return; while (array->lock) {
    } array->lock = 1; rsb_array__char* new_arr = RSBCreateArray_char(); for (size_t i = 0; i < array->len; i++) {
        if (i != index) {
            RSBAddElement_char(new_arr, RSBGetAtIndex_char(array, i));
        }
    } while (array->len != 0) {
        RSBPopElement_char(array);
    } for (size_t i = 0; i < new_arr->len; i++) {
        RSBAddElement_char(array, RSBGetAtIndex_char(new_arr, i));
    } RSBDestroy_char(new_arr); array->lock = 0;
}; void RSBClear_char(rsb_array__char* array) {
    if (!array || !array->objects || !array->len) return; while (array->lock) {
    } array->lock = 1; free(array->objects); array->len = 0; array->current_index = 0; array->added_elements = 0; array->lock = 0;
}; unsigned char RSBIsValid_char(rsb_array__char* array) {
    return array != ((void*)0) && array->objects != ((void*)0);
};;