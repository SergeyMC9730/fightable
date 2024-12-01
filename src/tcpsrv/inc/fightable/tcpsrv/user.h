#pragma once

#ifdef __cplusplus
extern "C" {
#endif

struct ftcp_server_user;

void _fTcpSrvUserSendMessage(struct ftcp_server_user *user, const char *message);

#ifdef __cplusplus
}
#endif
