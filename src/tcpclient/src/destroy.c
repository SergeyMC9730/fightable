#include <fightable/tcpcln/client.h>
#include <fightable/string.h>
#include <stdlib.h>
#ifdef TARGET_UNIX
#include <unistd.h>
#define NPD_CLOSE close
#elif defined(TARGET_WIN32)
#include <windows.h>
#include <winsock.h>
#include <io.h>
#define NPD_CLOSE _close
#endif
#include <stdio.h>

void _fTcpClientDestroy(struct ftcpclient *client) {
    if (!client) return;

    NPD_CLOSE(client->sockfd);
#ifdef TARGET_UNIX
    shutdown(client->sockfd, SHUT_RDWR);
#endif

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