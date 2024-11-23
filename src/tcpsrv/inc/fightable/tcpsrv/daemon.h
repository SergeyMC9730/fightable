#pragma once

struct ftcp_server_daemon;
struct ftcp_server_delegate;

#ifdef __cplusplus
extern "C" {
#endif

struct ftcp_server_daemon *_fTcpSrvCreate(unsigned short port, struct ftcp_server_delegate *delegate);
void _fTcpSrvDestroy(struct ftcp_server_daemon *daemon);

#ifdef __cplusplus
}
#endif