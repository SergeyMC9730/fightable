#include <fightable/tcpcln/client.h>
#include <fightable/shared_config.h>

#ifdef TARGET_UNIX
#include <unistd.h>
#include <fightable/sockcompat.h>
#define SHUT_REASON SHUT_RDWR
#elif defined(TARGET_WIN32)
#include <windows.h>
#include <winsock.h>
#include <io.h>
#define SD_BOTH 2
#define SHUT_REASON SD_BOTH
#endif

#include <stdio.h>

void _fTcpClientDisconnect(struct ftcpclient* client) {
    if (FIGHTABLE_OUTPUT_ONLY_WARNINGS) printf("ftcpclient: disconnecting\n");

#ifndef TARGET_WIN32
    NPD_CLOSE(client->sockfd);
#else
    closesocket(client->sockfd);
#endif
    shutdown(client->sockfd, SHUT_REASON);
}