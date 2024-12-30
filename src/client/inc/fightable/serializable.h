#pragma once

#include <stdint.h>
#include <stddef.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint8_t* data;
    size_t size;
    size_t offset;
} fserializable;

fserializable fCreateSerializableObject(size_t size);
fserializable fLoadSerializableObjectFromMemory(uint8_t* data, size_t size);
fserializable fLoadSerializableObjectFromFile(const char* filename);

void fSaveSerializedObject(fserializable obj, const char* filename);

void fUnloadSerializableObject(fserializable* serializable);

void fSerializableAddData(fserializable* serializable, void* data, size_t size);
void fSerializableAddInt8(fserializable* serializable, int8_t data);
void fSerializableAddInt16(fserializable* serializable, int16_t data);
void fSerializableAddInt32(fserializable* serializable, int32_t data);
void fSerializableAddInt64(fserializable* serializable, int64_t data);

void fSerializableAddFloat(fserializable* serializable, float data);
void fSerializableAddDouble(fserializable* serializable, double data);
void fSerializableAddString(fserializable* serializable, const char* data);

void fSerializableGetData(fserializable* serializable, void* data, size_t size);
int8_t fSerializableGetInt8(fserializable* serializable);
int16_t fSerializableGetInt16(fserializable* serializable);
int32_t fSerializableGetInt32(fserializable* serializable);
int64_t fSerializableGetInt64(fserializable* serializable);

float fSerializableGetFloat(fserializable* serializable);
double fSerializableGetDouble(fserializable* serializable);
const char* fSerializableGetString(fserializable* serializable);

#ifdef __cplusplus
}
#endif