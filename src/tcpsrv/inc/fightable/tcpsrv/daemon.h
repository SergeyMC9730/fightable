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
struct ftcp_server_user* _fTcpSrvGetUserByIndex(struct ftcp_server_daemon* daemon, unsigned int idx);
struct ftcp_server_user* _fTcpSrvGetUserByDescriptor(struct ftcp_server_daemon* daemon, int descriptor);

void _fTcpSrvSendGlobalMsg(struct ftcp_server_daemon *daemon, const char *message);

unsigned int _fTcpSrvGetConnectedUsers(struct ftcp_server_daemon *daemon);

void _fTcpSrvSetMaxClientsPerIp(struct ftcp_server_daemon* daemon, unsigned int amount);
unsigned int _fTcpSrvGetMaxClientsPerIp(struct ftcp_server_daemon* daemon);

unsigned char _fTcpSrvReady(struct ftcp_server_daemon* daemon);

#ifdef __cplusplus
}
#endif