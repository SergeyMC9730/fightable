#include <fightable/tcpcln/client.h>
#include <fightable/time.h>
#include <fightable/string.h>
#include <rsb/rsb_array_cstr.h>

#include <string.h>
#include <strings.h>

#ifdef TARGET_UNIX
#include <unistd.h>
#elif defined(TARGET_WIN32)
#include <io.h>
#endif
#include <stdio.h>

#include <math.h>

void *_fTcpClientWriteThread(struct ftcpclient *client) {
    if (!client) return NULL;

    while (!client->thread_should_exit) {
        if (client->requested_messages->len != 0) {
            size_t total_len = 0;

            for (int i = 0; i < client->requested_messages->len; i++) {
                const char *msg = RSBGetAtIndex_cstr(client->requested_messages, i);
                size_t len = strlen(msg);
                
                size_t sim = total_len + len;
                if (sim >= client->buf_size) {
                    break;
                }

                memcpy(client->buf_w + total_len, msg, len);
                memset(client->buf_w + total_len + len, '|', 1);

                total_len += len;
            }

            if (total_len != 0) {
                int max = (int)fmin(client->buf_size, total_len) - 1;

                client->buf_w[max] = 0;
                int res = write(client->sockfd, client->buf_w, max);

                if (res < 0) {
                    printf("ftcpclient: write: fail (%d). tried to write %d bytes\n", res, max);
                }
            }

            RSBClear_cstr(client->requested_messages);
        } else {
            _fSleep(250);
        }
    }

    return NULL;
}

void *_fTcpClientReadThread(struct ftcpclient *client) {
    if (!client) return NULL;

    while (!client->thread_should_exit) {
        memset(client->buf_r, 0, client->buf_size);
        int n = read(client->sockfd, client->buf_r, client->buf_size);

        if (n < 0 || n == 0) {
            printf("ftcpclient: read: client read less than zero bytes (%d). closing thread\n", n);
        
            return NULL;
        }
        printf("ftcpclient: received %d bytes: splitting got ", n);
        
        rsb_array__pchar *message_container = _fSplitString(client->buf_r, '|');
        printf("%d entries\n", message_container->len);

        for (unsigned int i = 0; i < message_container->len; i++) {
            printf("%d: %s\n", i, RSBGetAtIndex_pchar(message_container, i));
        }

        const char *reply = "$";
        write(client->sockfd, reply, strlen(reply));
        
        _fCleanupSplittedString(message_container);
    }

    return NULL;
}