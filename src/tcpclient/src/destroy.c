#include <fightable/tcpcln/client.h>
#include <fightable/string.h>
#include <stdlib.h>
#ifdef TARGET_UNIX
#include <unistd.h>
#define NPD_CLOSE close
#define NPD_SHUTDOWN(fd, mode) shutdown(fd, mode)
#elif defined(TARGET_WIN32)
#include <windows.h>
#include <winsock.h>
#include <io.h>
#define NPD_CLOSE _close
#define NPD_SHUTDOWN(fd, mode)
#define SHUT_RDWR 0
#endif
#include <stdio.h>

void _fTcpClientDestroy(struct ftcpclient *client) {
    if (!client) return;

    NPD_CLOSE(client->sockfd);
    NPD_SHUTDOWN(client->sockfd, SHUT_RDWR);

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