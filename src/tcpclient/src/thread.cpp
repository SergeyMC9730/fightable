#include <fightable/tcpcln/client.h>
#include <fightable/time.h>
#include <fightable/string.h>
#include <fightable/tcpcln/delegate.h>
#include <fightable/sockcompat.h>
#include <fightable/shared_config.h>
#include <fightable/generic_tools.hpp>

#include <string.h>
#include <strings.h>

#ifdef TARGET_UNIX
#include <unistd.h>
#elif defined(TARGET_WIN32)
#include <io.h>
#endif
#include <stdio.h>
#include <math.h>
#include <string>
extern "C" {
    #include <md5.h>
}

#define BASE_COMMAND_ACK '$'

void* _fTcpClientWriteThread(struct ftcpclient* client) {
    if (!client) return NULL;

    while (!client->thread_should_exit) {
        if (client->requested_messages->len != 0) {
            std::string msg_to_send = "";

            for (int i = 0; i < client->requested_messages->added_elements; i++) {
                char* msg = client->requested_messages->objects[i];
                if ((long long)msg <= 0x1000) continue;

                size_t len = strlen(msg);
                if (FIGHTABLE_OUTPUT_ONLY_WARNINGS) printf("%d -> message %s with length %ld\n", i, msg, len);

                msg_to_send += std::string((const char*)msg) + "|";

                free(msg);
            }

            RSBDestroy_pchar(client->requested_messages); client->requested_messages = NULL;
            client->requested_messages = RSBCreateArray_pchar();

            if (!msg_to_send.empty()) {
                msg_to_send.pop_back();

                if (FIGHTABLE_OUTPUT_ONLY_WARNINGS) printf("sending message %s (len=%ld)\n", msg_to_send.c_str(), msg_to_send.length());

#ifndef TARGET_WIN32
                int res = NPD_WRITE(client->sockfd, msg_to_send.data(), msg_to_send.size());
#else
                int res = send(client->sockfd, msg_to_send.data(), msg_to_send.size(), 0);
#endif

                if (res < 0) {
                    printf("ftcpclient: write: fail (%d). tried to write %ld bytes. exiting\n", res, msg_to_send.size());
#ifdef TARGET_WIN32
                    printf("ftcpclient: send: winsock returned %d\n", WSAGetLastError());
#endif
                    continue;
                }
            }
        }
        else {
            _fSleep(10);
        }
    }

    return NULL;
}

typedef uint8_t fmd5_t[16];

void* _fTcpClientReadThread(struct ftcpclient* client) {
    if (!client) return NULL;

    while (!client->thread_should_exit) {
        memset(client->buf_r, 0, client->buf_size);
#ifndef TARGET_WIN32
        int n = NPD_READ(client->sockfd, client->buf_r, client->buf_size - 1);
#else
        int n = recv(client->sockfd, client->buf_r, client->buf_size - 1, 0);
#endif

        if (n < 0 || n == 0) {
            printf("ftcpclient: read: client read less than zero bytes (%d). closing thread\n", n);
#ifdef TARGET_WIN32
            printf("ftcpclient: recv: winsock returns %d\n", WSAGetLastError());
#endif

            return NULL;
        }

        if (FIGHTABLE_OUTPUT_ONLY_WARNINGS) printf("ftcpclient: received %d bytes: splitting got ", n);
        
        auto base = client->buf_r;
        std::string orig_hash{base, 12};
        std::string hash;
        
        {
    	    fmd5_t md5;
    	    md5String(base + 12, md5);
    	    hash = GenericTools::valueToHex<fmd5_t>(md5);
	    hash.erase(hash.length() - 20, 20);
        }
        
        if (orig_hash != hash) {
    	    printf("ftcpclient: invalid hash. %s(o) != %s(e)\n", orig_hash.c_str(), hash.c_str());
    	    continue;
        }
        
        base += 12;
        
        rsb_array__pchar* message_container = _fSplitString(base, '|');
        if (FIGHTABLE_OUTPUT_ONLY_WARNINGS) printf("%d entries\n", message_container->len);

        for (unsigned int i = 0; i < message_container->added_elements; i++) {
            char* data = RSBGetAtIndex_pchar(message_container, i);

            if (strlen(data) < 5) {
                printf("ftcpclient: server sent response in invalid format\n");
                continue;
            }

            char header[5] = { data[0], data[1], data[2], data[3], 0 };
            unsigned char header_found = 0;

            for (unsigned int j = 0; j < client->received_headers->len; j++) {
                char* loaded_header = RSBGetAtIndex_pchar(client->received_headers, j);
                if (strncmp(header, loaded_header, 4) == 0) {
                    // printf("ftcpclient: duplicate string found\n");
                    header_found = 1;
                    continue;
                }
            }

            if (!header_found) {
                // printf("ftcpclient: registering header %s\n", header);
                RSBAddElement_pchar(client->received_headers, _fCopyString(header));
            }
            else {
                continue;
            }

            memmove(data, data + 4, strlen(data) - 4);
            memset(data + strlen(data) - 4, 0, 4);

            if (FIGHTABLE_OUTPUT_ONLY_WARNINGS) printf("ftcpclient: %d -> %s\n", i, data);

            if (client->delegate != NULL && client->delegate->processReceive != NULL) {
                client->delegate->client = client;
                if (client->delegate->processReceive) client->delegate->processReceive(client->delegate, data);
            }
        }

        unsigned char res = _fTcpClientSendMsgChar(client, BASE_COMMAND_ACK);
        if (res == 0) {
            printf("ftcpclient: cound not send acknowledge packet\n");
            if (client->delegate->onError) {
                client->delegate->onError(client->delegate, "cound not send acknowledge packet", 4);
            }
        }

        _fCleanupSplittedString(message_container);
    }

    return NULL;
}