#include <fightable/tcpcln/client.h>
#include <fightable/sockcompat.h>
#include <fightable/string.h>
#include <stdlib.h>
#ifdef TARGET_UNIX
#include <unistd.h>
#define SHUT_REASON SHUT_RDWR
#elif defined(TARGET_WIN32)
#include <windows.h>
#include <winsock.h>
#include <io.h>
#define SD_BOTH 2
#define SHUT_REASON SD_BOTH
#endif
#include <stdio.h>

void _fTcpClientDestroy(struct ftcpclient *client) {
    if (!client) return;

#ifndef TARGET_WIN32
    NPD_CLOSE(client->sockfd);
#else
    closesocket(client->sockfd);
#endif
    shutdown(client->sockfd, SHUT_REASON);

    client->thread_should_exit = 1;
    pthread_cancel(client->read_thread);
    pthread_join(client->write_thread, NULL);

    if (client->buf_r) {
        free(client->buf_r);
    }

    _fCleanupSplittedString(client->received_headers);
    _fCleanupSplittedString(client->requested_messages);

    free(client);

    return;
}