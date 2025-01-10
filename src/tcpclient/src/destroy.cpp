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
#include <vector>

void _fTcpClientDestroy(struct ftcpclient* client) {
    if (!client) return;

    _fTcpClientDisconnect(client);

    client->thread_should_exit = 1;
#ifndef TARGET_ANDROID
    pthread_cancel(client->read_thread);
#endif
    pthread_join(client->write_thread, NULL);

    if (client->buf_r) {
        free(client->buf_r);
    }

    if (client->received_headers) {
        auto* v = (std::vector<char*>*)client->received_headers;
        delete v;
    }
    if (client->requested_messages) {
        auto* v = (std::vector<char*>*)client->requested_messages;
        delete v;
    }

    free(client);

    return;
}