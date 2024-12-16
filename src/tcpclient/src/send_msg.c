#include <fightable/tcpcln/client.h>
#include <string.h>
#include <stdio.h>

unsigned char _fTcpClientSendMsg(struct ftcpclient *client, const char *message) {
    if (!client || !client->requested_messages || !message) return 0;

    unsigned int len = strlen(message) + 1;

    unsigned char *dup = (unsigned char *)malloc(len);
    memcpy(dup, message, len);

    RSBAddElement_pchar(client->requested_messages, dup);

    return 1;
}