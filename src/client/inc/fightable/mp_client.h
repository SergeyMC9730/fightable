#pragma once

#ifndef _DISABLE_MP_SERVER_

#ifdef __cplusplus
extern "C" {
#endif

unsigned char _fMpClientConnect(const char* ip, unsigned short port);
void _fMpClientMenuDraw();
void _fMpClientSendPacket(unsigned char type, void *data);
void _fMpClientTick();
void _fMpClientPrepareAssets();
void _fMpClientDownloadAssets(const char **assets, unsigned int array_length, void (*on_download_success)(void*), void *on_download_success_ctx);
void _fMpClientSwitchAssets();
const char *_fMpClientGetNewDirectory();
void _fMpClientSwitchToLobby();
void _fMpClientDraw();

#ifdef __cplusplus
}
#endif

#endif
