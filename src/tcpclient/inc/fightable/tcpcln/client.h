#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <pthread.h>
#include <rsb/rsb_array_pchar.h>

#ifdef TARGET_UNIX
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#elif defined(TARGET_WIN32)
#include <winsock.h>
#endif

struct ftcpclient_delegate;
struct ftcpclient {
    pthread_t read_thread;
    pthread_t write_thread;

    unsigned char thread_should_exit;

    rsb_array__pchar *received_headers;
    rsb_array__pchar *requested_messages;

    int sockfd;
    struct sockaddr_in serveraddr;
    struct hostent *server;

    const char *hostname;
    unsigned short port;

    unsigned int buf_size;

    char *buf_r;

    struct ftcpclient_delegate *delegate;
};

struct ftcpclient *_fTcpClientCreate(const char *address, unsigned short port, struct ftcpclient_delegate *delegate);
void _fTcpClientDestroy(struct ftcpclient *client);

void *_fTcpClientWriteThread(struct ftcpclient *client);
void *_fTcpClientReadThread(struct ftcpclient *client);

// returns success of message sending
unsigned char _fTcpClientSendMsg(struct ftcpclient *client, const char *message);

// returns success of message sending
unsigned char _fTcpClientSendMsgChar(struct ftcpclient* client, char c);

void _fTcpClientDisconnect(struct ftcpclient* client);

#ifdef __cplusplus
}
#endif