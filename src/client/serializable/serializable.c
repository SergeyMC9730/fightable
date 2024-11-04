#include <fightable/serializable.h>
#include <raylib.h>
#include <assert.h>
#include <stdlib.h>

fserializable fCreateSerializableObject(size_t size) {
    return (fserializable){RL_MALLOC(size), size, 0};
}

fserializable fLoadSerializableObjectFromMemory(uint8_t* data, size_t size) {
    return (fserializable){data, size, 0};
}

fserializable fLoadSerializableObjectFromFile(const char* filename) {
    int file_size;
    uint8_t* data = LoadFileData(filename, &file_size);

    return (fserializable){data, (size_t)file_size, 0};
}

void fUnloadSerializableObject(fserializable* serializable) {
    free(serializable->data);
}

void fSerializableAddData(fserializable* serializable, void* data, size_t size) {
    assert(serializable->offset + size < serializable->size);
    for(int i = 0; i < size; i++, serializable->offset++) {
        serializable->data[serializable->offset] = *(uint8_t*)&data + i;
    }
}

void fSerializableAddString(fserializable* serializable, const char* data) {
    return fSerializableAddData(serializable, (void*)data, strlen(data) + 1);
}

void fSerializableAddInt8(fserializable* serializable, int8_t data) {
    return fSerializableAddData(serializable, &data, sizeof(data));
}

void fSerializableAddInt16(fserializable* serializable, int16_t data) {
    return fSerializableAddData(serializable, &data, sizeof(data));
}

void fSerializableAddInt32(fserializable* serializable, int32_t data) {
    return fSerializableAddData(serializable, &data, sizeof(data));
}

void fSerializableAddInt64(fserializable* serializable, int64_t data) {
    return fSerializableAddData(serializable, &data, sizeof(data));
}

void fSerializableAddFloat(fserializable* serializable, float data) {
    return fSerializableAddData(serializable, &data, sizeof(data));
}

void fSerializableAddDouble(fserializable* serializable, double data) {
    return fSerializableAddData(serializable, &data, sizeof(data));
}

void fSerializableGetData(fserializable* serializable, void* data, size_t size) {
    assert(serializable->offset + size < serializable->size);
    memcpy(data, serializable->data + serializable->offset, size);
    serializable->offset += size;
}

int8_t fSerializableGetInt8(fserializable* serializable) {
    int8_t ret;
    fSerializableGetData(serializable, &ret, sizeof(ret));

    return ret;
}

int16_t fSerializableGetInt16(fserializable* serializable) {
    int16_t ret;
    fSerializableGetData(serializable, &ret, sizeof(ret));

    return ret;
}

int32_t fSerializableGetInt32(fserializable* serializable) {
    int32_t ret;
    fSerializableGetData(serializable, &ret, sizeof(ret));

    return ret;
}

int64_t fSerializableGetInt64(fserializable* serializable) {
    int64_t ret;
    fSerializableGetData(serializable, &ret, sizeof(ret));

    return ret;
}

float fSerializableGetFloat(fserializable* serializable) {
    float ret;
    fSerializableGetData(serializable, &ret, sizeof(ret));

    return ret;
}

double fSerializableGetDouble(fserializable* serializable) {
    double ret;
    fSerializableGetData(serializable, &ret, sizeof(ret));

    return ret;
}

const char* fSerializableGetString(fserializable* serializable) {
    int length;
    uint8_t* ptr = serializable->data + serializable->offset;

    while(*ptr) assert(serializable->offset + length++ < serializable->size);
    const char* str = RL_MALLOC(length);
    strcpy(str, serializable->data + serializable->offset);
    serializable->offset += length;

    return str;
}