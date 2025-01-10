#include <fightable/tcpcln/client.h>
#include <string.h>
#include <stdio.h>
//extern "C" {
//    #include <md5.h>
//}
#include <string>
#include <fightable/generic_tools.hpp>
#include <vector>

// typedef uint8_t fmd5_t[16];

unsigned char _fTcpClientSendMsg(struct ftcpclient *client, const char *message) {
    if (!client || !client->requested_messages || !message) return 0;

    unsigned int len = strlen(message) + 1;
    char *dup = (char *)malloc(len);

    snprintf(dup, len, "%s", message);

    ((std::vector<char*>*)client->requested_messages)->push_back(dup);

    return 1;
}

unsigned char _fTcpClientSendMsgChar(struct ftcpclient* client, char c) {
    if (c == 0) return 0;

    const char _c[2] = { c, 0 };
    return _fTcpClientSendMsg(client, _c);
}