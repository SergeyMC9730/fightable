#include <stdio.h>
#include <fightable/tcpsrv/daemon.h>
#include <fightable/tcpsrv/delegate.h>
#include <fightable/tcpsrv/user.h>

struct Vector2 {
    float x;
    float y;
};
struct PlayersArr {
    struct Vector2 player;
} PlayersArr;
struct PlayersArr arrPlayer[16];
enum message {
    DFLT = 0, PLAYERS, SHOOT
};
void processDisconnect(struct ftcp_server_delegate *self, struct ftcp_server_user *user) {
    printf("* disconnect happened\n");
}
void setDaemon(struct ftcp_server_delegate *self, struct ftcp_server_daemon *daemon) {
    printf("* setting daemon ctx\n");
}
void processMessage(struct ftcp_server_delegate *self, struct ftcp_server_user *user, unsigned char *message, unsigned int len) {
    printf("* received message from user with len %d: \"%s\"\n", len, message);
    switch (message[0]) {
    case PLAYERS:
        arrPlayer[_fTcpSrvUserGetId(user)].player.x = message[1];
        arrPlayer[_fTcpSrvUserGetId(user)].player.y = message[2];
        break;
    }
}

int main() {
    printf("* starting tcp server on port 8000\n");

    struct ftcp_server_delegate delegate = {
        .processDisconnect = processDisconnect,
        .processMessage = processMessage,
        .setDaemon = setDaemon
    };
    
    struct ftcp_server_daemon *daemon = _fTcpSrvCreate(8000, &delegate);

    while(1) {
        for(int p = 0; p < 16; p++) {
            for(int i = 0; i < 16; i++) {
                // _fTcpSrvSendGlobalMsg()
            }
        }
    }
    printf("* destroying tcp server\n");
    _fTcpSrvDestroy(daemon);
    
    printf("* done\n");

    return 0;
}