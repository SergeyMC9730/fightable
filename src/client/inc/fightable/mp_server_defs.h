
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#define MP_CREATE_PACKET_CLASS_DEF(PACKET_STRUCT, FN_PREFIX)    \
struct PACKET_STRUCT *_fMpPacketCreate##FN_PREFIX(void);        \
void _fMpPacketDestroy##FN_PREFIX(struct PACKET_STRUCT *obj);   \
int _fMpPacketSerialize##FN_PREFIX(struct PACKET_STRUCT *obj, NBN_Stream *stream);

#define MP_CREATE_PACKET_CLASS_IMPL(PACKET_STRUCT, FN_PREFIX, SER_IMPL)                 \
struct PACKET_STRUCT *_fMpPacketCreate##FN_PREFIX(void) {                               \
    return (struct PACKET_STRUCT *)NBN_Allocator(sizeof(struct PACKET_STRUCT));         \
}                                                                                       \
void _fMpPacketDestroy##FN_PREFIX(struct PACKET_STRUCT *obj) {                          \
    if (obj) NBN_Deallocator(obj);                                                      \
}                                                                                       \
int _fMpPacketSerialize##FN_PREFIX(struct PACKET_STRUCT *obj, NBN_Stream *stream) {     \
    if (!obj || !stream) return 0;                                                      \
    SER_IMPL                                                                            \
    return 0;                                                                           \
}

#define MP_CREATE_PACKET_CLASS_FULL(PACKET_STRUCT, FN_PREFIX, SER_IMPL)     \
MP_CREATE_PACKET_CLASS_DEF(PACKET_STRUCT, FN_PREFIX)                        \
MP_CREATE_PACKET_CLASS_IMPL(PACKET_STRUCT, FN_PREFIX, SER_IMPL)             \

#define MP_PACKET_CLASS_ATTACH(MESSAGE_ID, FN_PREFIX) NBN_GameServer_RegisterMessage(MESSAGE_ID, (NBN_MessageBuilder)_fMpPacketCreate##FN_PREFIX, (NBN_MessageDestructor)_fMpPacketDestroy##FN_PREFIX, (NBN_MessageSerializer)_fMpPacketSerialize##FN_PREFIX)

struct fmp_metadata_req {
    unsigned int http_port;
    unsigned int max_players;
    unsigned int players_connected;
};
struct fmp_metadata_acquire {
    char pad[1];
};

#define MP_METADATA_REQ_ID                  0x00
#define MP_METADATA_ACQUIRE_ID              0x01

#define MP_CON_REJECT_BUSY                  0x00


#define MP_MAX_CLIENTS                      8
