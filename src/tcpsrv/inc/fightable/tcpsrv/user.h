#pragma once

#ifdef __cplusplus
extern "C" {
#endif

struct ftcp_server_user;

void _fTcpSrvUserSendMessage(struct ftcp_server_user *user, const char *message);

const char *_fTcpSrvUserGetName(struct ftcp_server_user *user);
int _fTcpSrvUserGetId(struct ftcp_server_user *user);
int _fTcpSrvUserGetDescriptor(struct ftcp_server_user *user);

void _fTcpSrvUserDisconnect(struct ftcp_server_user *user);

#ifdef __cplusplus
}
#endif
