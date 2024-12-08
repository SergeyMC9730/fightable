#include <fightable/tcpcln/client.h>
#include <fightable/tcpcln/delegate.h>
#include <stdio.h>
#include <string.h>

#ifndef DISABLE_MP_SERVER
#include <fightable/tcpsrv/daemon.h>
#include <fightable/tcpsrv/delegate.h>

struct ftcp_server_daemon *__server;
struct ftcp_server_delegate __server_delegate;
#endif

void processReceive(struct ftcpclient_delegate *self, struct ftcpclient *client, unsigned char *message, unsigned int len) {
    printf("[CLIENT] received %d bytes\n", len);
}

struct ftcpclient *__client = NULL;
struct ftcpclient_delegate __client_delegate;

int tryToConnect();

#include <raylib.h>

int main() {
    int status = tryToConnect();
    
    if (status < 0) return status;
    
    InitWindow(800, 600, "Multiplayer Test");
    SetTargetFPS(60);
    
    while (!WindowShouldClose()) {
	BeginDrawing();
	ClearBackground(WHITE);
	EndDrawing();
    }
    
    _fTcpClientDestroy(__client);
#ifndef DISABLE_MP_SERVER
    _fTcpSrvDestroy(__server);
#endif
    
    return 0;
}

#ifndef DISABLE_MP_SERVER

#include <fightable/tcpsrv/user.h>

void processSrvDisconnect(struct ftcp_server_delegate *self, struct ftcp_server_user *user) {
    printf("[SERVER] disconnect: user %d has been disconnected\n", _fTcpSrvUserGetId(user));
}
void processSrvMessage(struct ftcp_server_delegate *self, struct ftcp_server_user *user, unsigned char *message, unsigned int len) {
    printf("[SERVER] received message from user with len %d: \"%s\"\n", len, message);
}
void setSrvDaemon(struct ftcp_server_delegate *self, struct ftcp_server_daemon *daemon) {

}

void createLocalServer() {
    printf("* starting tcp server on 0.0.0.0:8000\n");
    
    struct ftcp_server_delegate delegate = {
	.processDisconnect = processSrvDisconnect,
	.processMessage = processSrvMessage,
	.setDaemon = setSrvDaemon
    };
    __server_delegate = delegate;
    
    __server = _fTcpSrvCreate(8000, &__server_delegate);
    
    printf("* done\n");
}
#endif

int tryToConnect() {
    printf("* trying to connect to 127.0.0.1:8000\n");

    struct ftcpclient_delegate delegate = {
        .processReceive = processReceive
    };
    __client_delegate = delegate;
    __client = _fTcpClientCreate("127.0.0.1", 8000, &__client_delegate);

    if (!__client) {
        printf("* could not connect to 127.0.0.1:8000\n");
        
#ifdef DISABLE_MP_SERVER
	printf("* unrecoverable failure (DISABLE_MP_SERVER is defined)\n");
	return -1;
#else
	printf("* opening tcp server and trying to connect to it\n");
	createLocalServer();
	return tryToConnect();
#endif
    }

    printf("* done\n");

    return 0;
}