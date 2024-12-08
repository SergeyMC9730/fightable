#include <fightable/tcpcln/client.h>
#include <fightable/tcpcln/delegate.h>
#include <stdio.h>
#include <string.h>
#include <raylib.h>

#define PLAYER_WIDTH 10
#define PLAYER_HEIGHT 10
#define MAX_PLAYERS 16

enum message {
    DFLT = 0, PLAYERS, SHOOT
};

struct PlayersArr {
    Vector2 player;
} PlayersArr;

struct PlayersArr arrPlayer[16];

void processReceive(struct ftcpclient_delegate *self, struct ftcpclient *client, unsigned char *message, unsigned int len) {
    printf("* received %d bytes\n", len);
    switch(message[0]) {
        case PLAYERS:
            
            break;
    }
}
void sendPlayer(Vector2 player) {

};
int main() {
    printf("* trying to connect to localhost:8000\n");

    struct ftcpclient_delegate delegate = {
        .processReceive = processReceive
    };

    const char* ip = "127.0.0.1";

    struct ftcpclient *client = _fTcpClientCreate("127.0.0.1", 8000, &delegate);
    printf("* client has been created\n* press enter to stop\n");

    if (!client) {
        printf("* tcp client could not be created\n");
        return 1;
    }

    InitWindow(640, 480, "TCP Client");
    
    Rectangle player = {0, 0, PLAYER_WIDTH, PLAYER_HEIGHT};
    while (!WindowShouldClose()) {
        
        BeginDrawing();
            ClearBackground(RAYWHITE);

            DrawRectangleRec(player, MAROON);
            for(int i = 0; i < MAX_PLAYERS; i++) {
                DrawRectangle(arrPlayer[i].player.x, arrPlayer[i].player.y, PLAYER_WIDTH, PLAYER_HEIGHT, BLUE);
            }
            DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

        EndDrawing();
    }
    printf("* destroying tcp client\n");
    _fTcpClientDestroy(client);

    printf("* done\n");

    return 0;
}