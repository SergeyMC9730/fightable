#include <fightable/tcpcln/client.h>
#include <fightable/tcpcln/delegate.h>
#include <stdio.h>
#include <string.h>

void processReceive(struct ftcpclient_delegate *self, const char *message) {
    printf("* received %d bytes: %s\n", strlen(message) + 1, message);
}

int main() {
    printf("* trying to connect to localhost:8000\n");

    struct ftcpclient_delegate delegate = {
        .processReceive = processReceive
    };

    struct ftcpclient *client = _fTcpClientCreate("127.0.0.1", 8000, &delegate);

    if (!client) {
        printf("* tcp client could not be created\n");

        return 1;
    }

    printf("* client has been created\n* press enter to stop\n");

    printf("* sending test messages\n");

    _fTcpClientSendMsg(client, "Hello");
    _fTcpClientSendMsg(client, ", fricking");
    _fTcpClientSendMsg(client, " world!");

    char buffer[16] = {0};
    fgets(buffer, 16, stdin);

    printf("* destroying tcp client\n");
    _fTcpClientDestroy(client);

    printf("* done\n");

    return 0;
}