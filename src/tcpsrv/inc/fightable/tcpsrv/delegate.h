#pragma once

#ifdef __cplusplus
extern "C" {
#endif

struct ftcp_server_user;
struct ftcp_server_daemon;

struct ftcp_server_delegate {
    void (*processDisconnect)(struct ftcp_server_delegate *self, struct ftcp_server_user *user);
    void (*processConnect)(struct ftcp_server_delegate* self, struct ftcp_server_user* user);
    void (*setDaemon)(struct ftcp_server_delegate *self, struct ftcp_server_daemon *daemon);
    void (*processMessage)(struct ftcp_server_delegate *self, struct ftcp_server_user *user, unsigned char *message, unsigned int len);
};

#ifdef __cplusplus
}
#endif