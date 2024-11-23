#include <stdio.h>
#include <fightable/tcpsrv/daemon.h>
#include <fightable/tcpsrv/delegate.h>

void processDisconnect(struct ftcp_server_delegate *self, struct ftcp_server_user *user) {
    printf("* disconnect happened\n");
}
void setDaemon(struct ftcp_server_delegate *self, struct ftcp_server_daemon *daemon) {
    printf("* setting daemon ctx\n");
}
void processMessage(struct ftcp_server_delegate *self, struct ftcp_server_user *user, unsigned char *message, unsigned int len) {
    printf("* received message from user with len %d\n", len);
}

int main() {
    printf("* starting tcp server on port 8000\n");

    struct ftcp_server_delegate delegate = {
        .processDisconnect = processDisconnect,
        .processMessage = processMessage,
        .setDaemon = setDaemon
    };

    struct ftcp_server_daemon *daemon = _fTcpSrvCreate(8000, &delegate);

    printf("* press enter to stop\n");

    char buffer[16] = {0};
    fgets(buffer, 16, stdin);

    printf("* destroying tcp server\n");
    _fTcpSrvDestroy(daemon);
    
    printf("* done\n");

    return 0;
}