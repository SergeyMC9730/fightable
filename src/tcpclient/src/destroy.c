#include <fightable/tcpcln/client.h>
#include <fightable/string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

void _fTcpClientDestroy(struct ftcpclient *client) {
    if (!client) return;

    close(client->sockfd);
    shutdown(client->sockfd, SHUT_RDWR);

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