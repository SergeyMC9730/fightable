
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#ifndef _DISABLE_MP_SERVER_

#include <fightable/mp_shared.h>

#define MP_CREATE_PACKET_CLASS_DEF(PACKET_STRUCT, FN_PREFIX)    \
struct PACKET_STRUCT *_fMpPacketCreate##FN_PREFIX(void);        \
void _fMpPacketDestroy##FN_PREFIX(struct PACKET_STRUCT *obj);   \
int _fMpPacketSerialize##FN_PREFIX(struct PACKET_STRUCT *obj, NBN_Stream *stream);

#define MP_CREATE_PACKET_CLASS_IMPL(PACKET_STRUCT, FN_PREFIX, SER_IMPL)                 \
struct PACKET_STRUCT *_fMpPacketCreate##FN_PREFIX(void) {                               \
    struct PACKET_STRUCT * a = NBN_Allocator(sizeof(struct PACKET_STRUCT));             \
    memset(a, 0, sizeof(struct PACKET_STRUCT));                                         \
    return a;                                                                           \
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

#define MP_PACKET_CLASS_ATTACH(IS_CLIENT, MESSAGE_ID, FN_PREFIX) if (IS_CLIENT) { NBN_GameClient_RegisterMessage(MESSAGE_ID, (NBN_MessageBuilder)_fMpPacketCreate##FN_PREFIX, (NBN_MessageDestructor)_fMpPacketDestroy##FN_PREFIX, (NBN_MessageSerializer)_fMpPacketSerialize##FN_PREFIX); } else { NBN_GameServer_RegisterMessage(MESSAGE_ID, (NBN_MessageBuilder)_fMpPacketCreate##FN_PREFIX, (NBN_MessageDestructor)_fMpPacketDestroy##FN_PREFIX, (NBN_MessageSerializer)_fMpPacketSerialize##FN_PREFIX); }

#define MP_PACKET_SERIALIZE_ARRAY(ARRAY) NBN_SerializeBytes(stream, ARRAY, sizeof(ARRAY))

#include <fightable/mp_packets.h>

#define MP_CON_REJECT_BUSY                      0x00

#endif
