#include <fightable/state.h>
#include <fightable/mp_create_menu.h>
#include <fightable/storage.h>
#include <stdio.h>

#ifndef _DISABLE_MP_SERVER_
#include <fightable/http/http_server.h>
#include <fightable/tcpsrv/user.h>
#endif

extern void _fMainLog(const char* msg);

#define COMMAND_GET_UID         0x01
#define COMMAND_GET_USERS       0x02
#define COMMAND_ACKNOWLEDGE     '$'

#ifndef _DISABLE_MP_SERVER_
void _fMpCreateOnConnect(struct ftcp_server_delegate* self, struct ftcp_server_user* user) {
	TraceLog(LOG_INFO, "Connection happened");
}
void _fMpCreateOnDisconnect(struct ftcp_server_delegate* self, struct ftcp_server_user* user) {
	TraceLog(LOG_INFO, "Disconnection happened");
}
void _fMpCreateOnMessage(struct ftcp_server_delegate* self, struct ftcp_server_user* user, unsigned char* message, unsigned int len) {
	TraceLog(LOG_INFO, "TCP message: \"%s\" (%d)", message, len);

    char command = message[0];
    if (command == COMMAND_ACKNOWLEDGE) return;

    switch (command) {
    case COMMAND_GET_UID: {
        char buffer[16] = {};
        snprintf(buffer, 16, "%c%d", command, _fTcpSrvUserGetId(user));

        _fTcpSrvUserSendMessage(user, buffer);

        break;
    }
    case COMMAND_GET_USERS: {
        //int i = 0;
        //struct ftcp_server_user* remuser = _fTcpSrvGetUserByIndex(self->daemon, i);

        //size_t min_alloc_size = 64;
        //size_t cur_alloc_size = min_alloc_size;

        //char* result = (char*)MemAlloc(cur_alloc_size);
        //result[0] = command;

        //while (remuser != NULL) {
        //    Vector2 pos = { 0, 0 };
        //    int uid = _fTcpSrvUserGetId(remuser);
        //    int pl_idx = SrvGetPlayerIndex(uid);

        //    if (pl_idx >= 0) {
        //        struct example_player player = RSBGetAtIndex_example_player(__server_players, pl_idx);
        //        pos = player.pos;
        //    }
        //    else {
        //        printf("[SERVER] player %d not found (%d)\n", uid, pl_idx);
        //    }

        //    char* buf = (char*)MemAlloc(256);
        //    snprintf(buf, 256, "%d,%s,%d,%d;", uid, _fTcpSrvUserGetNameEncrypted(remuser), (int)pos.x, (int)pos.y);

        //    while (strlen(buf) >= cur_alloc_size) {
        //        cur_alloc_size += min_alloc_size;
        //        result = (char*)MemRealloc(result, cur_alloc_size);
        //    }

        //    strcat(result, buf);

        //    MemFree(buf);

        //    remuser = _fTcpSrvGetUserByIndex(self->daemon, ++i);
        //}

        //result[strlen(result) - 1] = 0; // remove last character

        //_fTcpSrvUserSendMessage(user, result);

        //MemFree(result);

        break;
    }
    default: {
        printf("[SERVER] unknown command %d\n", command);
        break;
    }
    }
}
#endif

void _fMpCreateOpenServer() {
#ifndef _DISABLE_MP_SERVER_
	__state.mp_server_delegate.processConnect = _fMpCreateOnConnect;
	__state.mp_server_delegate.processDisconnect = _fMpCreateOnDisconnect;
	__state.mp_server_delegate.processMessage = _fMpCreateOnMessage;

	unsigned char create_successful = false;

	for (int i = 0; i < 16; i++) {
		__state.mp_server_port = GetRandomValue(1024, 8000);
		__state.mp_server_port = 8000;
		__state.mp_server_instance = _fTcpSrvCreate(__state.mp_server_port, &__state.mp_server_delegate);

		if (!__state.mp_server_instance || !_fTcpSrvReady(__state.mp_server_instance)) {
			TraceLog(LOG_WARNING, "Failed to open TCP server. Attempt %d", i + 1);

			if (__state.mp_server_instance) {
				_fTcpSrvDestroy(__state.mp_server_instance);
			}
		}
		else {
			create_successful = true;
			break;
		}
	}

	if (!create_successful) {
		TraceLog(LOG_ERROR, "Could not start TCP server with 16 attempts");

		return;
	}

	__state.webserver = _fHttpServerCreate(3000, _fMainLog);

#ifdef TARGET_ANDROID
	_fHttpSetAllowedResourceDir(__state.webserver, __state.system->activity->internalDataPath);
#else
	_fHttpSetAllowedResourceDir(__state.webserver, _fStorageGetWritable());
#endif
#endif
}