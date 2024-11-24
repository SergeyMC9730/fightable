#include <fightable/tcpcln/client.h>
#include <fightable/time.h>
#include <fightable/string.h>
#include <fightable/tcpcln/delegate.h>

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
                char *msg = RSBGetAtIndex_pchar(client->requested_messages, i);
                size_t len = strlen(msg);
                
                size_t sim = total_len + len;
                if (sim >= client->buf_size) {
                    break;
                }

                memcpy(client->buf_w + total_len, msg, len);
                memset(client->buf_w + total_len + len, '|', 1);

                total_len += len;

                free(msg);
            }

            if (total_len != 0) {
                int max = (int)fmin(client->buf_size, total_len) - 1;

                client->buf_w[max] = 0;
                int res = write(client->sockfd, client->buf_w, max);

                if (res < 0) {
                    printf("ftcpclient: write: fail (%d). tried to write %d bytes\n", res, max);
                }
            }

            RSBClear_pchar(client->requested_messages);
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
            char *data = RSBGetAtIndex_pchar(message_container, i);

            if (strlen(data) < 5) {
                printf("ftcpclient: server sent response in invalid format\n");
                continue;
            }

            char header[5] = {data[0], data[1], data[2], data[3], 0};
            unsigned char header_found = 0;

            for (unsigned int j = 0; j < client->received_headers->len; j++) {
                char *loaded_header = RSBGetAtIndex_pchar(client->received_headers, j);
                if (strncmp(header, loaded_header, 4) == 0) {
                    // printf("ftcpclient: duplicate string found\n");
                    header_found = 1;
                    continue;
                }
            }

            if (!header_found) {
                // printf("ftcpclient: registering header %s\n", header);
                RSBAddElement_pchar(client->received_headers, _fCopyString(header));
            } else {
                continue;
            }

            memmove(data, data + 4, strlen(data) - 4);
            memset(data + strlen(data) - 4, 0, 4);

            printf("ftcpclient: %d -> %s\n", i, data);

            if (client->delegate != NULL && client->delegate->processReceive != NULL) {
                client->delegate->processReceive(client->delegate, client, data, strlen(data) + 1);
            }
        }

        const char *reply = "$";
        write(client->sockfd, reply, strlen(reply));

        // unsigned char res = _fTcpClientSendMsg(client, "$", 2);
        // if (res == 0) {
        //     printf("ftcpclient: cound not send read packet\n");
        // }

        _fCleanupSplittedString(message_container);
    }

    return NULL;
}