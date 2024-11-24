#include <fightable/tcpcln/client.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <strings.h>
#include <netdb.h>
#include <unistd.h>

struct ftcpclient *_fTcpClientCreate(const char *address, unsigned short port, struct ftcpclient_delegate *delegate) {
    if (delegate == NULL) {
        printf("ftcpclient: delegate cannot be NULL\n");
        return NULL;
    }
    
    struct ftcpclient *client = malloc(sizeof(struct ftcpclient));
    memset(client, 0, sizeof(struct ftcpclient));

    client->hostname = address;
    client->port = port;

    client->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    if (client->sockfd < 0) {
        printf("ftcpclient: socket: fail (%d)\n", client->sockfd);

        free(client);
        return NULL;
    }

    client->server = gethostbyname(client->hostname);
    if (client->server == NULL) {
        printf("ftcpclient: gethostbyname: fail (%s)\n", client->hostname);
        
        close(client->sockfd);
        free(client);

        return NULL;
    }

    bzero((char *)&client->serveraddr, sizeof(client->serveraddr));
    client->serveraddr.sin_family = AF_INET;
    bcopy((char *)client->server->h_addr_list[0], (char *)&client->serveraddr.sin_addr.s_addr, client->server->h_length);
    client->serveraddr.sin_port = htons(port);

    int res = connect(client->sockfd, (const struct sockaddr *)&client->serveraddr, sizeof(client->serveraddr));
    if (res < 0) {
        printf("ftcpclient: connect: fail (%d)\n", res);

        close(client->sockfd);
        free(client);

        return NULL;
    }


    client->buf_size = 65536;
    client->buf_r = (char *)malloc(client->buf_size);
    client->buf_w = (char *)malloc(client->buf_size);

    client->received_headers = RSBCreateArray_cstr();
    client->requested_messages = RSBCreateArray_cstr();

    client->delegate = delegate;

    pthread_create(&client->read_thread, NULL, (void*(*)(void *))_fTcpClientReadThread, client);
    pthread_create(&client->write_thread, NULL, (void*(*)(void *))_fTcpClientWriteThread, client);

    return client;
}