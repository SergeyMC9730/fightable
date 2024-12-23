#pragma once

#ifdef __cplusplus
extern "C" {
#endif

struct ftcp_server_user;

void _fTcpSrvUserSendMessage(struct ftcp_server_user *user, const char *message);

const char *_fTcpSrvUserGetName(struct ftcp_server_user *user);
const char* _fTcpSrvUserGetNameEncrypted(struct ftcp_server_user* user);
int _fTcpSrvUserGetId(struct ftcp_server_user *user);
int _fTcpSrvUserGetDescriptor(struct ftcp_server_user *user);

void _fTcpSrvUserDisconnect(struct ftcp_server_user *user);

void _fTcpSrvUserSetName(struct ftcp_server_user *user, const char *username);
void _fTcpSrvUserSetNameEncrypted(struct ftcp_server_user *user, const char *username);

#ifdef __cplusplus
}
#endif
