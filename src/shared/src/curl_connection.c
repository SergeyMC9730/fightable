
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "fightable/pthread_compat.h"
#include <fightable/curl_frontend.h>
#include <curl/curl.h>
#include <pthread.h>
#include <string.h>

static size_t _fCurlConnectionWriteData(void *ptr, size_t size, size_t nmemb, void *stream) {
    struct fcurl_con_settings *settings = (struct fcurl_con_settings *)stream;

    if(settings->output_to_buffer) {
        //if(settings->sendDebug) printf("size: %d nmemb: %d | msize: %d mx size: %d\n", (int)size, (int)nmemb, settings->m_nSize, settings->m_nMaxMemorySize);
        if(nmemb > settings->output_buffer_size - settings->size) {
            settings->output_buffer_data = realloc(settings->output_buffer_data, settings->output_buffer_size + nmemb + 1);
            settings->output_buffer_size = settings->output_buffer_size + nmemb;
        }
        int i = 0;
        char *dd = (char *)settings->output_buffer_data;
        char *d1 = (char *)ptr;
        while(i < nmemb) {
            dd[settings->size + i] = d1[i];
            i++;
        }
        settings->size += nmemb;
        return nmemb;
    } else {
        size_t written = fwrite(ptr, size, nmemb, settings->output_file);
        settings->size += written;
        return written;
    }
}

int _fCurlConnectionProgress(void *bar, double dltotal, double dlnow, double ultotal, double ulnow) {
    struct fcurl_con_settings *settings = (struct fcurl_con_settings *)bar;
    settings->result->download_progress = ulnow / ultotal;

    return 0;
}

struct fcurl_connection *_fCurlConnectionCreate() {
    struct fcurl_connection *con = (struct fcurl_connection *)malloc(sizeof(struct fcurl_connection));

    con->instance = curl_easy_init();
    con->debug = 1;
    con->user_data = RSBCreateArrayCurlParameter();
    con->current_operation = NULL;

    return con;
}
void _fCurlConnectionDestroy(struct fcurl_connection *con) {
    if (!con) return;

    if (!_fComparePthreadAndEmptyThread(con->result_thread)) {
        pthread_join(con->result_thread, NULL);
    }
    if (con->current_operation) {
        if (con->current_operation->result) {
            struct fcurl_result *r = con->current_operation->result;

            if (r->data) {
                free(r->data);
            }
            free(r);
        }
        if (con->current_operation->output_file) {
            fclose(con->current_operation->output_file);
        }
        free(con->current_operation);
    }
    if (con->instance) {
        curl_easy_cleanup(con->instance);
    }
    if (con->user_data) {
        _fCurlConnectionClearParameters(con);
        RSBDestroyCurlParameter(con->user_data);
    }
    free(con);
}

void _fCurlConnectionAddParameter(struct fcurl_connection *con, const char *key, const char *value) {
    if (!con || !con->user_data || !key || !value) return;

    RSBAddElementCurlParameter(con->user_data, _fCurlParameterCreateWithString(key, value));
}
void _fCurlConnectionAddParameterInt(struct fcurl_connection *con, const char *key, int value) {
    if (!con || !con->user_data || !key) return;

    RSBAddElementCurlParameter(con->user_data, _fCurlParameterCreateWithInteger(key, value));
}
void _fCurlConnectionClearParameters(struct fcurl_connection *con) {
    if (!con || !con->user_data) return;

    for (unsigned int i = 0; i < con->user_data->len; i++) {
        struct fcurl_parameter param = RSBGetAtIndexCurlParameter(con->user_data, i);
        _fCurlParameterDestroy(&param);
    }

    RSBClearCurlParameter(con->user_data);
}

struct fcurl_con_context {
    struct fcurl_connection *con;
    const char *url;
    const char *method;
    void (*async_callback)(struct fcurl_con_settings *);
    PTHREAD_TYPE thread;
    FILE *file;
};

void *_fCurlConnectionThr0(void *pctx) {
    struct fcurl_con_context *ctx = (struct fcurl_con_context *)pctx;
    struct fcurl_con_settings *settings = _fCurlConnectionOpenPageWithMethod(ctx->con, ctx->url, ctx->method, 0, ctx->async_callback);

    free(ctx);

    return NULL;
}
void *_fCurlConnectionThr1(void *pctx) {
    printf("fcurl: Threaded curl operation begin\n");

    struct fcurl_con_context *ctx = (struct fcurl_con_context *)pctx;
    struct fcurl_con_settings *settings = _fCurlConnectionDownloadPageWithMethod(ctx->con, ctx->url, ctx->method, ctx->file, 0, ctx->async_callback);

    free(ctx);

    return NULL;
}

struct fcurl_con_settings *_fCurlConnectionOpenPage(struct fcurl_connection *con, const char *url, unsigned char async, void (*async_callback)(struct fcurl_con_settings *)) {
    return _fCurlConnectionOpenPageWithMethod(con, url, "GET", async, async_callback);
}
struct fcurl_con_settings *_fCurlConnectionDownloadPage(struct fcurl_connection *con, const char *url, FILE *output, unsigned char async, void (*async_callback)(struct fcurl_con_settings *)) {
    return _fCurlConnectionDownloadPageWithMethod(con, url, "GET", output, async, async_callback);
}
struct fcurl_con_settings *_fCurlConnectionOpenPageWithMethod(struct fcurl_connection *con, const char *url, const char *method, unsigned char async, void (*async_callback)(struct fcurl_con_settings *)) {
    if (!con || !url || !method || con->current_operation) return NULL;

    if (async) {
        struct fcurl_con_context *ctx = (struct fcurl_con_context *)malloc(sizeof(struct fcurl_con_context));

        ctx->async_callback = async_callback;
        ctx->con = con;
        ctx->method = method;
        ctx->url = url;

        pthread_create(&con->result_thread, NULL, _fCurlConnectionThr0, ctx);

        while (!con->current_operation) {}
        printf("con->current_operation=%p", con->current_operation);
        return con->current_operation;
    }

    unsigned int max_size = 512 * 1024;

    struct fcurl_con_settings *settings = (struct fcurl_con_settings *)calloc(1, sizeof(struct fcurl_con_settings));
    struct fcurl_result *res = (struct fcurl_result *)calloc(1, sizeof(struct fcurl_result));

    settings->result = res;
    settings->output_to_buffer = 1;

    curl_easy_setopt(con->instance, CURLOPT_URL, url);
    curl_easy_setopt(con->instance, CURLOPT_CUSTOMREQUEST, method);
    curl_easy_setopt(con->instance, CURLOPT_WRITEFUNCTION, _fCurlConnectionWriteData);
    curl_easy_setopt(con->instance, CURLOPT_WRITEDATA, settings);
    curl_easy_setopt(con->instance, CURLOPT_VERBOSE, con->debug);
    curl_easy_setopt(con->instance, CURLOPT_TIMEOUT, 10);
    curl_easy_setopt(con->instance, CURLOPT_NOPROGRESS, 0);
    curl_easy_setopt(con->instance, CURLOPT_PROGRESSFUNCTION, _fCurlConnectionProgress);
    curl_easy_setopt(con->instance, CURLOPT_PROGRESSDATA, settings);

    char *generated_user_data = NULL;

    if (con->user_data) {
        generated_user_data = (char *)calloc(1, 8192);
        char *temp = (char *)calloc(1, 8192);

        for (unsigned int i = 0; i < con->user_data->len; i++) {
            struct fcurl_parameter param = RSBGetAtIndexCurlParameter(con->user_data, i);
            if (!param.key || !param.value) continue;

            snprintf(temp, 8192, "%s=%s", param.key, param.value);

            if (strlen(temp) > (8192 - strlen(generated_user_data))) break;

            strcat(generated_user_data, temp);
        }

        free(temp);

        if (con->user_data->len != 0) {
            curl_easy_setopt(con->instance, CURLOPT_POSTFIELDS, generated_user_data);
        }
    }

    con->current_operation = settings;

    int result = (int)curl_easy_perform(con->instance);
    res->result = result;
    res->data = settings->output_buffer_data;
    res->max_size = max_size;

    if (res->result != CURLE_OK) {
        if (async_callback) {
            async_callback(settings);
            _fCurlConnectionDestroyResult(settings);

            con->current_operation = NULL;

            return NULL;
        }

        return settings;
    }

    int http_status = 500;
    curl_off_t ra;

    curl_easy_getinfo(con->instance, CURLINFO_RESPONSE_CODE, &http_status);
    curl_easy_getinfo(con->instance, CURLINFO_RETRY_AFTER, &ra);

    res->http_status = http_status;
    res->retry_after = (int)ra;
    // printf("[CURL] retry after %d seconds\n", (int)ra);
    res->max_size = settings->output_buffer_size;
    res->real_size = settings->size;

    if (async_callback) {
        async_callback(settings);
        _fCurlConnectionDestroyResult(settings);

        con->current_operation = NULL;

        return NULL;
    }

    con->current_operation = NULL;

    return settings;
}
struct fcurl_con_settings *_fCurlConnectionDownloadPageWithMethod(struct fcurl_connection *con, const char *url, const char *method, FILE *output, unsigned char async, void (*async_callback)(struct fcurl_con_settings *)) {
    if (!con || !url || !method || !output || con->current_operation) return NULL;

    if (async) {
        struct fcurl_con_context *ctx = (struct fcurl_con_context *)malloc(sizeof(struct fcurl_con_context));

        ctx->async_callback = async_callback;
        ctx->con = con;
        ctx->method = method;
        ctx->url = url;
        ctx->file = output;

        pthread_create(&con->result_thread, NULL, _fCurlConnectionThr1, ctx);

        while (!con->current_operation) {}
        printf("con->current_operation=%p", con->current_operation);
        return con->current_operation;
    }

    unsigned int max_size = 512 * 1024;

    struct fcurl_con_settings *settings = (struct fcurl_con_settings *)calloc(1, sizeof(struct fcurl_con_settings));
    struct fcurl_result *res = (struct fcurl_result *)calloc(1, sizeof(struct fcurl_result));

    settings->result = res;
    settings->output_file = output;

    curl_easy_setopt(con->instance, CURLOPT_URL, url);
    curl_easy_setopt(con->instance, CURLOPT_CUSTOMREQUEST, method);
    curl_easy_setopt(con->instance, CURLOPT_WRITEFUNCTION, _fCurlConnectionWriteData);
    curl_easy_setopt(con->instance, CURLOPT_WRITEDATA, settings);
    curl_easy_setopt(con->instance, CURLOPT_VERBOSE, con->debug);
    curl_easy_setopt(con->instance, CURLOPT_TIMEOUT, 10);
    curl_easy_setopt(con->instance, CURLOPT_NOPROGRESS, 0);
    curl_easy_setopt(con->instance, CURLOPT_PROGRESSFUNCTION, _fCurlConnectionProgress);
    curl_easy_setopt(con->instance, CURLOPT_PROGRESSDATA, settings);

    char *generated_user_data = NULL;

    if (con->user_data) {
        generated_user_data = (char *)calloc(1, 8192);
        char *temp = (char *)calloc(1, 8192);

        for (unsigned int i = 0; i < con->user_data->len; i++) {
            struct fcurl_parameter param = RSBGetAtIndexCurlParameter(con->user_data, i);
            if (!param.key || !param.value) continue;

            snprintf(temp, 8192, "%s=%s", param.key, param.value);

            if (strlen(temp) > (8192 - strlen(generated_user_data))) break;

            strcat(generated_user_data, temp);
        }

        free(temp);

        if (con->user_data->len != 0) {
            curl_easy_setopt(con->instance, CURLOPT_POSTFIELDS, generated_user_data);
        }
    }

    con->current_operation = settings;

    int result = (int)curl_easy_perform(con->instance);
    res->result = result;
    res->data = settings->output_buffer_data;
    res->max_size = max_size;

    if (res->result != CURLE_OK) {
        if (async_callback) {
            async_callback(settings);
            _fCurlConnectionDestroyResult(settings);

            con->current_operation = NULL;

            return NULL;
        }

        return settings;
    }

    int http_status = 500;
    curl_off_t ra;

    curl_easy_getinfo(con->instance, CURLINFO_RESPONSE_CODE, &http_status);
    curl_easy_getinfo(con->instance, CURLINFO_RETRY_AFTER, &ra);

    res->http_status = http_status;
    res->retry_after = (int)ra;
    // printf("[CURL] retry after %d seconds\n", (int)ra);
    res->max_size = settings->output_buffer_size;
    res->real_size = settings->size;

    if (async_callback) {
        async_callback(settings);
        _fCurlConnectionDestroyResult(settings);

        con->current_operation = NULL;

        return NULL;
    }

    con->current_operation = NULL;

    return settings;
}

void _fCurlConnectionDestroyResult(struct fcurl_con_settings *r) {
    if (r) {
        if (r->result) {
            if (r->result->data) {
                free(r->result->data);
            }
            free(r->result);
        }
        free(r);
    }
}
