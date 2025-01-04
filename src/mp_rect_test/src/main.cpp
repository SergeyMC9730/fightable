#include <fightable/tcpcln/client.h>
#include <fightable/tcpcln/delegate.h>
#include <fightable/string.h>
#include <fightable/shared_config.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <raylib.h>

static std::string __ip;
static unsigned short __port = 5000;

struct example_player {
    int uid;
    Vector2 pos;
    Vector2 old_pos;
};

RSB_ARRAY_DEF_GEN(struct example_player, _example_player);
RSB_ARRAY_IMPL_GEN(struct example_player, _example_player);

#ifndef DISABLE_MP_SERVER
#include <fightable/tcpsrv/daemon.h>
#include <fightable/tcpsrv/delegate.h>

struct ftcp_server_daemon *__server;
struct ftcp_server_delegate __server_delegate;
rsb_array__example_player* __server_players;

int SrvGetPlayerIndex(int uid) {
    for (int i = 0; i < __server_players->len; i++) {
        struct example_player player = RSBGetAtIndex_example_player(__server_players, i);
        if (player.uid == uid) return i;
    }

    return -1;
}
#endif
rsb_array__example_player* __client_players;
int ClnGetPlayerIndex(int uid) {
    for (int i = 0; i < __client_players->len; i++) {
        struct example_player player = RSBGetAtIndex_example_player(__client_players, i);
        if (player.uid == uid) return i;
    }

    return -1;
}

int __userId = -1;

#define CURRENT_PROTOCOL_VERSION "fightable-1"

#define COMMAND_GET_UID         0x01
#define COMMAND_GET_USERS       0x02
#define COMMAND_VERIFY_VER      'b'
#define COMMAND_SET_POS         0x03
#define COMMAND_GLOB_SET_POS    0x04

// #define NO_SERVER_CHECK

void processReceive(struct ftcpclient_delegate *self, const char *message) {
    if (FIGHTABLE_OUTPUT_ONLY_WARNINGS) printf("[CLIENT] received %ld bytes: %s\n", strlen(message) + 1, message);
    
    char response_cmd = message[0];
    
    switch (response_cmd) {
	case COMMAND_GET_UID: {
        message++;

	    __userId = atoi(message);
        struct example_player player = {
            .uid = __userId,
            .pos = (Vector2){0, 0}
        };

        RSBAddElement_example_player(__client_players, player);

	    break;
	}
    case COMMAND_VERIFY_VER: {
        message += 2;

        if (strcmp(message, CURRENT_PROTOCOL_VERSION)) {
            printf("[CLIENT] fatal error: incorrent protocol version. expected %s; got: %s\n", CURRENT_PROTOCOL_VERSION, message);
            _fTcpClientDisconnect(self->client);
        }

        break;
    }
    case COMMAND_GET_USERS: {
        message++;

        rsb_array__pchar* array1 = _fSplitString((const char *)message, ';');
        if (!array1) return;

        for (unsigned int i = 0; i < array1->added_elements; i++) {
            char* user_object_str = RSBGetAtIndex_pchar(array1, i);
            rsb_array__pchar* array2 = _fSplitString((const char*)user_object_str, ',');
            if (!array2) continue;
            if (array2->added_elements < 4) {
                printf("[CLIENT] expected array2 to have at least 4 objects; got: %d\n", array2->added_elements);
                continue;
            }

            int uid = atoi(array2->objects[0]);
            const char* ename = array2->objects[1];
            Vector2 pos = { (float)atoi(array2->objects[2]), (float)atoi(array2->objects[3]) };

            int idx = ClnGetPlayerIndex(uid);
            if (idx < 0) {
                if (FIGHTABLE_OUTPUT_ONLY_WARNINGS) printf("[CLIENT] NEW PLAYER (%d)\n", uid);

                struct example_player player = {
                    .uid = uid,
                    .pos = pos
                };
                RSBAddElement_example_player(__client_players, player);
            }
            else if (uid != __userId) {
                struct example_player* ref = __client_players->objects + idx;
                ref->pos = pos;
            }
        }

        break;
    }
	default: {
	    printf("[CLIENT] unknown response command %d\n", (int)response_cmd);
	    break;
	}
    }
}

struct ftcpclient *__client = NULL;
struct ftcpclient_delegate __client_delegate;

bool __stop_thread = false;

int tryToConnect();

#include <raylib.h>

void getUserId() {
    _fTcpClientSendMsgChar(__client, COMMAND_GET_UID);
}
void getOtherUsers() {
    _fTcpClientSendMsgChar(__client, COMMAND_GET_USERS);
}
void sendPlayerInfo(Vector2 pos) {
    char* buffer = (char*)MemAlloc(64);
    snprintf(buffer, 64, "%c%d,%d", COMMAND_SET_POS, (int)pos.x, (int)pos.y);

    _fTcpClientSendMsg(__client, buffer);

    MemFree(buffer);
}

#include <fightable/time.h>
#include <thread>
#include <chrono>

void ClnThread(void *ctx) {
    while (__client != NULL && !__stop_thread) {
        int self_idx = ClnGetPlayerIndex(__userId);
        auto ref = __client_players->objects + self_idx;

        if (memcmp(&ref->old_pos, &ref->pos, sizeof(Vector2)) != 0) {
            // printf("PLAYER POS UPDATED\n");
            sendPlayerInfo(ref->pos);
        }

        ref->old_pos = ref->pos;

	// printf("in loop\n");

	getOtherUsers();
        
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

int main(int argc, char **argv) {
    if (argc > 1) {
        __ip = argv[1];
        if (argc > 2) {
            std::string port = argv[2];
            __port = std::stoi(port);
        }

        printf("--- CLIENT WANTS TO CONNECT TO %s:%d\n", __ip.c_str(), __port);
    }

#ifndef DISABLE_MP_SERVER
    __server_players = RSBCreateArray_example_player();
#endif
    __client_players = RSBCreateArray_example_player();

    int status = tryToConnect();
    
    if (status < 0) return status;
    
    InitWindow(800, 600, "Multiplayer Test");
    SetTargetFPS(GetMonitorRefreshRate(0));

    getUserId();
    getOtherUsers();

    SetTextLineSpacing((int)(15.f / GetWindowScaleDPI().y * 1.5f));

    std::thread *thr = nullptr;
    bool thread_initialized = false;
    
    while (!WindowShouldClose()) {
        if (__userId != -1 && !thread_initialized) {
            int self_idx = ClnGetPlayerIndex(__userId);
            if (self_idx == -1) {
                printf("[CLIENT] cannot start network thread\n");
            }
            else {
                struct example_player* ref = __client_players->objects + self_idx;
                if (thr == nullptr) thr = new std::thread([ref]() {
            	    ClnThread(ref);
                });
            }
        }

	    BeginDrawing();
	    ClearBackground(RAYWHITE);
	
	    char buffer[20] = {};
	
#ifndef DISABLE_MP_SERVER
	    int clients = _fTcpSrvGetConnectedUsers(__server);
	
	    snprintf(buffer, 16, "clients: %d", clients);
	    RlDrawText(buffer, 4, 600 - 20, 20, GREEN);
#endif
	    snprintf(buffer, 16, "user id: %d", __userId);
	    RlDrawText(buffer, 4, 4, 20, GREEN);

        int self_idx = ClnGetPlayerIndex(__userId);
        if (self_idx < 0) {
            RlDrawText("player could not be loaded", 120, 4, 20, RED);
        }
        else {
            struct example_player* ref = __client_players->objects + self_idx;
            Vector2* refpos = &ref->pos;

            float delta = GetFrameTime();
            float speed = delta * 140.f;

            if (IsKeyDown(KEY_W)) {
                refpos->y -= speed;
            } 
            else if (IsKeyDown(KEY_S)) {
                refpos->y += speed;
            }

            if (IsKeyDown(KEY_A)) {
                refpos->x -= speed;
            }
            else if (IsKeyDown(KEY_D)) {
                refpos->x += speed;
            }
        }

        for (unsigned int i = 0; i < __client_players->added_elements; i++) {
            struct example_player player = RSBGetAtIndex_example_player(__client_players, i);

            Color col = (player.uid == __userId) ? GREEN : RED;
            DrawRectangleV(player.pos, (Vector2) { 20, 20 }, col);
        }

	    EndDrawing();
    }
    
    __stop_thread = true;
    if (thread_initialized) {
	thr->join();
	delete thr;
    }

    _fTcpClientDestroy(__client);
#ifndef DISABLE_MP_SERVER
    _fTcpSrvDestroy(__server);
#endif

    __client = NULL;

    RlCloseWindow();
    
    return 0;
}

#ifndef DISABLE_MP_SERVER

#include <fightable/tcpsrv/user.h>

void processSrvDisconnect(struct ftcp_server_delegate *self, struct ftcp_server_user *user) {
    int uid = _fTcpSrvUserGetId(user);
    printf("[SERVER] disconnect: user %d has been disconnected\n", uid);

    bool should_cleanup = true;
    if (!should_cleanup) return;

    int idx = SrvGetPlayerIndex(uid);
    if (idx < 0) return;

    RSBPopElementAtIndex_example_player(__server_players, idx);
}
void processConnect(struct ftcp_server_delegate* self, struct ftcp_server_user* user) {
    int uid = _fTcpSrvUserGetId(user);
    if (FIGHTABLE_OUTPUT_ONLY_WARNINGS) printf("[SERVER] connection from %d\n", uid);
    
    int idx = SrvGetPlayerIndex(uid);
    if (idx == -1) {
        if (1) printf("[SERVER] %d: NEW PLAYER\n", uid);
        struct example_player player = {
            .uid = uid,
            .pos = (Vector2){0, 0}
        };

        RSBAddElement_example_player(__server_players, player);
    }
}
void processSrvMessage(struct ftcp_server_delegate *self, struct ftcp_server_user *user, unsigned char *message, unsigned int len) {
    // int uid = _fTcpSrvUserGetId(user);
    // if (1) printf("[SERVER] received message from user %d with len %d: \"%s\"\n", uid, len, message);
    
    char command = message[0];
    if (command == '$') return;
    
    switch (command) {
	case COMMAND_GET_UID: {
	    char buffer[16] = {};
	    snprintf(buffer, 16, "%c%d", command, _fTcpSrvUserGetId(user));
	
	    _fTcpSrvUserSendMessage(user, buffer);
	    
	    break;
	}
    case COMMAND_GET_USERS: {
        int i = 0;
        struct ftcp_server_user* remuser = _fTcpSrvGetUserByIndex(self->daemon, i);

        size_t min_alloc_size = 64;
        size_t cur_alloc_size = min_alloc_size;

        char* result = (char*)MemAlloc(cur_alloc_size);
        result[0] = command;

        while (remuser != NULL) {
            Vector2 pos = { 0, 0 };
            int uid = _fTcpSrvUserGetId(remuser);
            int pl_idx = SrvGetPlayerIndex(uid);
            
            if (pl_idx >= 0) {
                struct example_player player = RSBGetAtIndex_example_player(__server_players, pl_idx);
                pos = player.pos;
            }
            else {
                printf("[SERVER] player %d not found (%d)\n", uid, pl_idx);
            }

            char* buf = (char*)MemAlloc(256);
            snprintf(buf, 256, "%d,%s,%d,%d;", uid, _fTcpSrvUserGetNameEncrypted(remuser), (int)pos.x, (int)pos.y);

            while (strlen(buf) >= cur_alloc_size) {
                cur_alloc_size += min_alloc_size;
                result = (char*)MemRealloc(result, cur_alloc_size);
            }

            strcat(result, buf);

            MemFree(buf);

            remuser = _fTcpSrvGetUserByIndex(self->daemon, ++i);
        }

        result[strlen(result) - 1] = 0; // remove last character

        _fTcpSrvUserSendMessage(user, result);

        MemFree(result);

        break;
    }
    case COMMAND_SET_POS: {
        message++;

        int uid = _fTcpSrvUserGetId(user);
        // int desc = _fTcpSrvUserGetDescriptor(user);
        
        int pl_idx = SrvGetPlayerIndex(uid);

        if (pl_idx < 0) {
            printf("[SERVER] player object for %d COULD NOT be found\n", uid);
            break;
        }
        
        struct example_player* ref = __server_players->objects + pl_idx;
        rsb_array__pchar* entries = _fSplitString((const char *)message, ',');

        if (entries->added_elements < 2) {
            printf("[SERVER] incomplete request from user %d\n", uid);
            break;
        }

        Vector2 new_pos = { (float)atoi(entries->objects[0]), (float)atoi(entries->objects[1]) };
        ref->pos = new_pos;

        // if (1) printf("[SERVER] NEW POS: %f %f for player with id %d(%d)\n", new_pos.x, new_pos.y, uid, desc);
        
        break;
    }
	default: {
	    printf("[SERVER] unknown command %d\n", command);
	    break;
	}
    }
}

void createLocalServer() {
    if (1) printf("* starting tcp server on 0.0.0.0:%d\n", __port);
    
    struct ftcp_server_delegate delegate = {
	    .processDisconnect = processSrvDisconnect,
	    .processMessage = processSrvMessage,
        .processConnect = processConnect
    };
    __server_delegate = delegate;
    
    __server = _fTcpSrvCreate(__port, &__server_delegate);
    
    if (1) printf("* done\n");
}
#endif

int tryToConnect() {
#if defined(NO_SERVER_CHECK) && !defined(DISABLE_MP_SERVER)
    createLocalServer();
#endif
    if (1) printf("* trying to connect to %s:%d\n", __ip.c_str(), __port);

    struct ftcpclient_delegate delegate = {
        .processReceive = processReceive
    };
    __client_delegate = delegate;
    __client = _fTcpClientCreate(__ip.c_str(), __port, &__client_delegate);

    if (!__client) {
        printf("* could not connect to %s:%d\n", __ip.c_str(), __port);
        
#ifdef DISABLE_MP_SERVER
	printf("* unrecoverable failure (DISABLE_MP_SERVER is defined)\n");
	return -1;
#else
        if (1) printf("* opening tcp server and trying to connect to it\n");

	    createLocalServer();
	    return tryToConnect();
#endif
    }

    if (1) printf("* done\n");

    return 0;
}