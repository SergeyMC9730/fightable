#pragma once

struct ftcp_server_daemon;
struct ftcp_server_delegate;
struct ftcp_server_user;

#ifdef __cplusplus
extern "C" {
#endif

struct ftcp_server_daemon *_fTcpSrvCreate(unsigned short port, struct ftcp_server_delegate *delegate);
void _fTcpSrvDestroy(struct ftcp_server_daemon *daemon);

struct ftcp_server_user *_fTcpSrvGetUserByName(struct ftcp_server_daemon *daemon, const char *username);
struct ftcp_server_user *_fTcpSrvGetUserById(struct ftcp_server_daemon *daemon, int user_id);

void _fTcpSrvSendGlobalMsg(struct ftcp_server_daemon *daemon, const char *message);

unsigned int _fTcpSrvGetConnectedUsers(struct ftcp_server_daemon *daemon);

#ifdef __cplusplus
}
#endif