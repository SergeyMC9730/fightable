#pragma once

#ifdef __cplusplus
extern "C" {
#endif

struct ftcpclient;

struct ftcpclient_delegate {
    void (*processReceive)(struct ftcpclient_delegate *self, struct ftcpclient *client, const char *message);
};

#ifdef __cplusplus
}
#endif