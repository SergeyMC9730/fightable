#include <rsb/rsb_array_pchar.h>

rsb_array__pchar* RSBCreateArray_pchar() {
    rsb_array__pchar* array = (rsb_array__pchar*)calloc(1, sizeof(rsb_array__pchar)); array->len = 0; array->current_index = 0; array->added_elements = 0; array->objects = ((void*)0); return array;
}; rsb_array__pchar* RSBCreateArrayFromList_pchar(char** object_list, unsigned int size) {
    rsb_array__pchar* array = RSBCreateArray_pchar(); if (object_list == ((void*)0)) return array; for (unsigned int i = 0; i < size; i++) {
        RSBAddElement_pchar(array, object_list[i]);
    } return array;
}; void RSBDestroy_pchar(rsb_array__pchar* array) {
    if (!array || !array->objects) return; free(array->objects); free(array); return;
}; void RSBAddElement_pchar(rsb_array__pchar* array, char* object) {
    if (!array || array->lock == 0xdd) return; while (array->lock) {
    } array->lock = 1; if (!array->objects) {
        array->objects = (char**)calloc(1, sizeof(char*)); array->len = 1; array->current_index = 0; array->added_elements = 0;
    } if ((array->added_elements + 1) > array->len) {
        array->objects = (char**)realloc(array->objects, sizeof(char*) * (array->len + 1)); array->len++;
    } array->objects[array->current_index] = object; array->added_elements++; array->current_index++; array->lock = 0; return;
}; void RSBPopElement_pchar(rsb_array__pchar* array) {
    if (!array || !array->objects || array->added_elements == 0 || array->len == 0) return; while (array->lock) {
    } array->lock = 1; if ((array->len - 1) == 0) {
        free(array->objects); array->objects = ((void*)0); array->len--; array->added_elements--; return;
    } array->objects = (char**)realloc(array->objects, sizeof(char*) * (array->len - 1)); array->len--; array->added_elements--; if (array->current_index > array->len) {
        if (array->current_index != 0) array->current_index--;
    } array->lock = 0; return;
}; void RSBMergeElements_pchar(rsb_array__pchar* source, rsb_array__pchar* destination) {
    if (!source || !destination || !source->objects || !destination->objects) return; while (destination->lock) {
    } destination->lock = 1; for (size_t i = 0; i < source->len; i++) {
        char* obj = RSBGetAtIndex_pchar(source, i); RSBAddElement_pchar(destination, obj);
    } destination->lock = 0; return;
}; char* RSBGetAtIndex_pchar(rsb_array__pchar* array, unsigned int index) {
    (void)((!!(!(!array || !array->objects || array->len < index))) || (_wassert(L"!(!array || !array->objects || array->len < index)", L"C:\\Users\\dogot\\fightable\\src\\shared\\src\\pchar.c", (unsigned)(3)), 0)); while (array->lock) {
    } return array->objects[index];
}; void RSBAddElementAtIndex_pchar(rsb_array__pchar* array, char* object, unsigned int index) {
    if (!array || index > array->len) return; while (array->lock) {
    } array->lock = 1; rsb_array__pchar* new_arr = RSBCreateArray_pchar(); for (size_t i = 0; i < array->len; i++) {
        if (i == index) {
            RSBAddElement_pchar(new_arr, object);
        } RSBAddElement_pchar(new_arr, RSBGetAtIndex_pchar(array, i));
    } while (array->len != 0) {
        RSBPopElement_pchar(array);
    } for (size_t i = 0; i < new_arr->len; i++) {
        RSBAddElement_pchar(array, RSBGetAtIndex_pchar(new_arr, i));
    } RSBDestroy_pchar(new_arr); array->lock = 0;
}; void RSBPopElementAtIndex_pchar(rsb_array__pchar* array, unsigned int index) {
    if (!array || index > array->len) return; while (array->lock) {
    } array->lock = 1; rsb_array__pchar* new_arr = RSBCreateArray_pchar(); for (size_t i = 0; i < array->len; i++) {
        if (i != index) {
            RSBAddElement_pchar(new_arr, RSBGetAtIndex_pchar(array, i));
        }
    } while (array->len != 0) {
        RSBPopElement_pchar(array);
    } for (size_t i = 0; i < new_arr->len; i++) {
        RSBAddElement_pchar(array, RSBGetAtIndex_pchar(new_arr, i));
    } RSBDestroy_pchar(new_arr); array->lock = 0;
}; void RSBClear_pchar(rsb_array__pchar* array) {
    if (!array || !array->objects || !array->len) return; while (array->lock) {
    } array->lock = 1; free(array->objects); array->len = 0; array->current_index = 0; array->added_elements = 0; array->lock = 0;
}; unsigned char RSBIsValid_pchar(rsb_array__pchar* array) {
    return array != ((void*)0) && array->objects != ((void*)0);
};;