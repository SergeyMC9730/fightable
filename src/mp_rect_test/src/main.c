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

#include <raylib.h>

RLRectangle __remotePlayers[0x0f] = {};
RLRectangle __localPlayer = {};

int __userId = -1;

#define CURRENT_PROTOCOL_VERSION "fightable-1"

void processReceive(struct ftcpclient_delegate *self, const char *message) {
    printf("[CLIENT] received %d bytes: %s\n", strlen(message) + 1, message);
    
    char response_cmd = message[0];
    
    switch (response_cmd) {
	case '0': {
	    const char *value = message + 1;
	    __userId = atoi(value);
	    break;
	}
    case 'b': {
        const char* value = message + 2;

        if (strcmp(value, CURRENT_PROTOCOL_VERSION)) {
            printf("[CLIENT] fatal error: incorrent protocol version. expected %s; got: %s\n", CURRENT_PROTOCOL_VERSION, value);
            _fTcpClientDisconnect(self->client);
        }

        break;
    }
	default: {
	    printf("[CLIENT] unknown response command %c\n", response_cmd);
	    break;
	}
    }
}

struct ftcpclient *__client = NULL;
struct ftcpclient_delegate __client_delegate;

int tryToConnect();

#include <raylib.h>

void initLocalPlayer() {
    RLRectangle r = {0, 0, 32, 32};
    __localPlayer = r;
}

void getUserId() {
    _fTcpClientSendMsg(__client, "0");
}

int main() {
    int status = tryToConnect();
    
    if (status < 0) return status;
    
    InitWindow(800, 600, "Multiplayer Test");
    SetTargetFPS(60);
    
    initLocalPlayer();
    getUserId();

    SetTextLineSpacing((int)(15.f / GetWindowScaleDPI().y * 1.5f));
    
    while (!WindowShouldClose()) {
	    if (IsKeyPressed(KEY_I)) {
	        getUserId();
	    }
    
	    BeginDrawing();
	    ClearBackground(RAYWHITE);
	
	    char buffer[20] = {};
	
#ifndef DISABLE_MP_SERVER
	    int clients = _fTcpSrvGetConnectedUsers(__server);
	
	    snprintf(buffer, 16, "clients: %d", clients);
	    RlDrawText(buffer, 4, 600 - 20, 20, GREEN);
#endif
	    snprintf(buffer, 16, "uid: %d", __userId);
	    RlDrawText(buffer, 4, 4, 20, GREEN);
	

	    EndDrawing();
    }
    
    _fTcpClientDestroy(__client);
#ifndef DISABLE_MP_SERVER
    _fTcpSrvDestroy(__server);
#endif

    RlCloseWindow();
    
    return 0;
}

#ifndef DISABLE_MP_SERVER

#include <fightable/tcpsrv/user.h>

void processSrvDisconnect(struct ftcp_server_delegate *self, struct ftcp_server_user *user) {
    printf("[SERVER] disconnect: user %d has been disconnected\n", _fTcpSrvUserGetId(user));
}
void processSrvMessage(struct ftcp_server_delegate *self, struct ftcp_server_user *user, unsigned char *message, unsigned int len) {
    printf("[SERVER] received message from user with len %d: \"%s\"\n", len, message);
    
    char command = message[0];
    if (command == '$') return;
    
    switch (command) {
	case '0': {
	    char buffer[16] = {};
	    snprintf(buffer, 16, "0%d", _fTcpSrvUserGetId(user));
	
	    _fTcpSrvUserSendMessage(user, buffer);
	    
	    break;
	}
	default: {
	    printf("[SERVER] unknown command %c\n", command);
	    break;
	}
    }
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