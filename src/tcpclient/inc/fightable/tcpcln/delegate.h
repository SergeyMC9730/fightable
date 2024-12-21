#pragma once

#ifdef __cplusplus
extern "C" {
#endif

struct ftcpclient;

struct ftcpclient_delegate {
    struct ftcpclient *client;

    void (*processReceive)(struct ftcpclient_delegate *self, const char *message);
    void (*onError)(struct ftcpclient_delegate *self, const char *message, int error_id);
    void (*onConnect)(struct ftcpclient_delegate *self);
};

#ifdef __cplusplus
}
#endif