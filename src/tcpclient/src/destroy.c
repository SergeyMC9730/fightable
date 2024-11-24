#include <fightable/tcpcln/client.h>
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
    if (client->buf_w) {
        free(client->buf_w);
    }

    if (client->received_headers) {
        RSBDestroy_cstr(client->received_headers);
    }
    if (client->requested_messages) {
        RSBDestroy_cstr(client->requested_messages);
    }

    free(client);

    return;
}