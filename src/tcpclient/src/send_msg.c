#include <fightable/tcpcln/client.h>
#include <string.h>

unsigned char _fTcpClientSendMsg(struct ftcpclient *client, unsigned char *data, unsigned int len) {
    if (!client || !RSBIsValid_pchar(client->requested_messages)) return 0;

    unsigned char *dup = (unsigned char *)malloc(len);
    memcpy(dup, data, len);

    dup[len - 1] = 0;

    RSBAddElement_pchar(client->requested_messages, dup);

    return 1;
}