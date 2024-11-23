#include <fightable/tcpcln/client.h>
#include <rsb/rsb_array_cstr.h>

#include <string.h>
#include <strings.h>

#include <unistd.h>
#include <stdio.h>

#include <math.h>

void *_fTcpClientReadThread(struct ftcpclient *client) {
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
            
        }
    }
}