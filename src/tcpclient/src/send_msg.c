#include <fightable/tcpcln/client.h>
#include <string.h>
#include <stdio.h>

unsigned char _fTcpClientSendMsg(struct ftcpclient *client, const char *message) {
    if (!client || !client->requested_messages || !message) return 0;

    unsigned int len = strlen(message) + 1;

    unsigned char *dup = (unsigned char *)malloc(len);
    memset(dup, 0, len);
    memcpy(dup, message, len - 1);

    RSBAddElement_pchar(client->requested_messages, dup);

    return 1;
}

unsigned char _fTcpClientSendMsgChar(struct ftcpclient* client, char c) {
    if (c == 0) return 0;

    const char _c[2] = { c, 0 };
    return _fTcpClientSendMsg(client, _c);
}