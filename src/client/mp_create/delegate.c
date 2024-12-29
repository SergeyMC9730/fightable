#include <fightable/state.h>
#include <fightable/mp_create_menu.h>

void _fMpCreateOnConnect(struct ftcp_server_delegate* self, struct ftcp_server_user* user) {
	TraceLog(LOG_INFO, "Connection happened");
}
void _fMpCreateOnDisconnect(struct ftcp_server_delegate* self, struct ftcp_server_user* user) {
	TraceLog(LOG_INFO, "Disconnection happened");
}
void _fMpCreateOnMessage(struct ftcp_server_delegate* self, struct ftcp_server_user* user, unsigned char* message, unsigned int len) {
	TraceLog(LOG_INFO, "TCP message: \"%s\" (%d)", message, len);
}

void _fMpCreateOpenServer() {
	__state.mp_server_delegate.processConnect = _fMpCreateOnConnect;
	__state.mp_server_delegate.processDisconnect = _fMpCreateOnDisconnect;
	__state.mp_server_delegate.processMessage = _fMpCreateOnMessage;

	unsigned char create_successful = false;

	for (int i = 0; i < 16; i++) {
		__state.mp_server_port = GetRandomValue(1024, 8000);
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
	}
}