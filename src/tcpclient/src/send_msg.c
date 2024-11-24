#include <fightable/tcpcln/client.h>
#include <string.h>
#include <stdio.h>

unsigned char _fTcpClientSendMsg(struct ftcpclient *client, unsigned char *data, unsigned int len) {
    if (!client || !client->requested_messages || !data || !len || len >= client->buf_size) return 0;

    unsigned char *dup = (unsigned char *)malloc(len);
    memcpy(dup, data, len);

    dup[len - 1] = 0;

    RSBAddElement_pchar(client->requested_messages, dup);

    return 1;
}