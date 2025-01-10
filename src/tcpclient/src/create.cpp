#include <fightable/tcpcln/client.h>
#include <fightable/tcpcln/delegate.h>
#include <fightable/shared_config.h>
#include <vector>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <strings.h>
#ifdef TARGET_UNIX
#include <netdb.h>
#include <unistd.h>
#elif defined(TARGET_WIN32)
#include <io.h>
#endif

#include <fightable/sockcompat.h>

struct ftcpclient* _fTcpClientCreate(const char* address, unsigned short port, struct ftcpclient_delegate* delegate) {
    if (delegate == NULL) {
        printf("ftcpclient: delegate cannot be NULL\n");
        return NULL;
    }

    struct ftcpclient* client = (struct ftcpclient* )malloc(sizeof(struct ftcpclient));
    memset(client, 0, sizeof(struct ftcpclient));

    delegate->client = client;

    client->hostname = address;
    client->port = port;

    int res = 0;

#ifdef TARGET_WIN32
    WSADATA wsaData;

    res = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (FIGHTABLE_OUTPUT_ONLY_WARNINGS) printf("ftcpclient: WSAStartup returned %d\n", res);
    assert(res == 0 && "ftcpclient: WSAStartup: fail");
#endif

    client->sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (client->sockfd < 0) {
        printf("ftcpclient: socket: fail (%d)\n", client->sockfd);

        if (delegate->onError) delegate->onError(delegate, "socket() failed", 1);

        free(client);
        return NULL;
    }

    client->server = gethostbyname(client->hostname);
    if (client->server == NULL) {
        printf("ftcpclient: gethostbyname: fail (%s)\n", client->hostname);

        if (delegate->onError) delegate->onError(delegate, "gethostbyname() failed", 2);

#ifdef TARGET_WIN32
        closesocket(client->sockfd);
#else
        NPD_CLOSE(client->sockfd);
#endif
        free(client);

        return NULL;
    }

    memset((void*)&client->serveraddr, 0, sizeof(client->serveraddr));
    client->serveraddr.sin_family = AF_INET;
    memcpy((void *)&client->serveraddr.sin_addr.s_addr, (void *)client->server->h_addr_list[0], client->server->h_length);
    client->serveraddr.sin_port = htons(port);

    res = connect(client->sockfd, (const struct sockaddr*)&client->serveraddr, sizeof(client->serveraddr));
    if (res < 0) {
        printf("ftcpclient: connect: fail (%d)\n", res);

        if (delegate->onError) delegate->onError(delegate, "connect() failed", 3);

#ifdef TARGET_WIN32
        closesocket(client->sockfd);
#else
        NPD_CLOSE(client->sockfd);
#endif
        free(client);

        return NULL;
    }

    client->buf_size = 65536;
    client->buf_r = (char*)malloc(client->buf_size);;

    client->received_headers = new std::vector<char*>();
    client->requested_messages = new std::vector<char*>();

    client->delegate = delegate;

    pthread_create(&client->read_thread, NULL, (void* (*)(void*))_fTcpClientReadThread, client);
    pthread_create(&client->write_thread, NULL, (void* (*)(void*))_fTcpClientWriteThread, client);

    if (delegate->onConnect) delegate->onConnect(delegate);

    return client;
}