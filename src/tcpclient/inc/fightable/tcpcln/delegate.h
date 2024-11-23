#pragma once

#ifdef __cplusplus
extern "C" {
#endif

struct ftcpclient;

struct ftcpclient_delegate {
    void (*processReceive)(struct ftcpclient_delegate *self, struct ftcpclient *client, unsigned char *message, unsigned int len);
};

#ifdef __cplusplus
}
#endif