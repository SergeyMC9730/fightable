#pragma once

#ifndef _DISABLE_MP_SERVER_

#ifdef __cplusplus
extern "C" {
#endif

unsigned char _fMpClientConnect(const char* ip, unsigned short port);
void _fMpClientMenuDraw();
void _fMpClientSendPacket(unsigned char type, void *data);
void _fMpClientTick();

#ifdef __cplusplus
}
#endif

#endif
