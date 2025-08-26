
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <stdio.h>
#include <fightable/pthread_compat.h>

typedef void CURL;

// parameter of the POST request
struct fcurl_parameter {
    char *key;
    char *value;
};

// result of the curl call
struct fcurl_result {
    // data size received by curl
    unsigned int real_size;

    // result of the curl call
    int result;

    // max size of the data buffer
    unsigned int max_size;

    // http status
    int http_status;

    // ratelimit value: how much client should wait for the next request
    int retry_after;

    // data if present (same data buffer as in connection settings)
    char *data;

    // this flag is raised when download completes. useful in async operations
    unsigned char ready;

    // ratio of downloaded bytes to expected by curl bytes. goes from 0 to 1. useful in async operations
    double download_progress;
};

// connection settings
struct fcurl_con_settings {
    // write to the buffer
    unsigned char output_to_buffer;

    // file to write data
    FILE *output_file;

    // buffer to write data
    void *output_buffer_data;

    // buffer size
    unsigned int output_buffer_size;

    // size
    int size;

    // run in debug mode
    unsigned char debug;

    // output. pointer is set by curl frontend
    struct fcurl_result *result;

    /*
     * function is called only in async operation and when that operation is complete
     * NOTE: pointer is destroyed after callback
    */
    void (*result_callback)(struct fcurl_con_settings *);

    // custom data. you should it cleanup manually
    void *custom_data;
};

#include <rsb/rsb_array_gen.h>

RSB_ARRAY_DEF_GEN(struct fcurl_parameter, CurlParameter);

struct fcurl_connection {
    // curl instance
    CURL *instance;

    // debug flag
    unsigned char debug;

    // website parameters
    rsb_array_CurlParameter *user_data;

    // current operation done by curl
    struct fcurl_con_settings *current_operation;

    // required for any async operation. this is a thread in which an operation is being done
    PTHREAD_TYPE result_thread;
};

#ifdef __cplusplus
extern "C" {
#endif

struct fcurl_parameter _fCurlParameterCreateWithString(const char *key, const char *value);
struct fcurl_parameter _fCurlParameterCreateWithInteger(const char *key, int value);
void _fCurlParameterDestroy(struct fcurl_parameter *p);

struct fcurl_connection *_fCurlConnectionCreate();
void _fCurlConnectionDestroy(struct fcurl_connection *con);

struct fcurl_con_settings *_fCurlConnectionOpenPage(struct fcurl_connection *con, const char *url, unsigned char async, void (*async_callback)(struct fcurl_con_settings *));
struct fcurl_con_settings *_fCurlConnectionDownloadPage(struct fcurl_connection *con, const char *url, FILE *output, unsigned char async, void (*async_callback)(struct fcurl_con_settings *));
struct fcurl_con_settings *_fCurlConnectionOpenPageWithMethod(struct fcurl_connection *con, const char *url, const char *method, unsigned char async, void (*async_callback)(struct fcurl_con_settings *));
struct fcurl_con_settings *_fCurlConnectionDownloadPageWithMethod(struct fcurl_connection *con, const char *url, const char *method, FILE *output, unsigned char async, void (*async_callback)(struct fcurl_con_settings *));

void _fCurlConnectionAddParameter(struct fcurl_connection *con, const char *key, const char *value);
void _fCurlConnectionAddParameterInt(struct fcurl_connection *con, const char *key, int value);
void _fCurlConnectionClearParameters(struct fcurl_connection *con);

void _fCurlConnectionDestroyResult(struct fcurl_con_settings *r);

#ifdef __cplusplus
}
#endif
