
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#ifndef _DISABLE_MP_SERVER_

#include <fightable/state.h>
#include <fightable/mp_client.h>
#include <fightable/storage.h>
#include <nbnet.h>
#include <net_drivers/udp.h>
#include <fightable/mp_server.h>
#include <fightable/notif_mgr.h>
#include <fightable/string.h>
#include <stdio.h>
#include <fightable/curl_frontend.h>
#include <fightable/renderer.h>

struct fmp_download_context {
    const char **assets;
    unsigned int array_length;
    unsigned int current_file_index;
    struct fcurl_connection *connection;
    char *current_url;
};

void _fMpClientDownloadFile(struct fmp_download_context *ctx);

void _fMpClientRequestFileDownload(void *ctx) {
    if (!ctx) return;
    _fMpClientDownloadFile(ctx);
}
void _fMpClientOnDownloadedFile(struct fcurl_con_settings *settings) {
    if (!settings || !settings->custom_data) return;

    renderer_event_t e;
    e.callback = _fMpClientRequestFileDownload;
    e.user = settings->custom_data;
    _fScheduleOverlayFunc(e);
}
void _fMpClientRequestCurlDestruction(void *ctx) {
    if (!ctx) return;
    _fCurlConnectionDestroy(ctx);
}

void _fMpClientDownloadFile(struct fmp_download_context *ctx) {
    if (!ctx || ctx->current_file_index >= ctx->array_length) {
        TraceLog(LOG_ERROR, "Cannot download next file");

        renderer_event_t e;
        e.callback = _fMpClientRequestCurlDestruction;
        e.user = ctx->connection;
        _fScheduleOverlayFunc(e);

        free(ctx);
        return;
    }

    const char *dir = _fMpClientGetNewDirectory();
    const char *file = ctx->assets[ctx->current_file_index];

    unsigned int buffer_size = strlen(dir) + strlen(file) + 2;
    char *buffer = (char *)malloc(buffer_size);

    snprintf(buffer, buffer_size, "%s/%s", dir, file);

    TraceLog(LOG_INFO, "Downloading %s", buffer);

    FILE *f = fopen(buffer, "wb");
    
    char *url_buffer = (char *)malloc(512);
    memset(url_buffer, 0, 512);
    
    snprintf(url_buffer, 511, "http://%s:%d/api/v1/res/request/%s", __state.mp_client_ip, __state.mp_client_srvmeta->http_port, file);

    if (!ctx->connection) {
        ctx->connection = _fCurlConnectionCreate();
    }
    struct fcurl_con_settings *settings = _fCurlConnectionDownloadPage(ctx->connection, url_buffer, f, 1, _fMpClientOnDownloadedFile);
    settings->custom_data = ctx;
    ctx->current_file_index++;

    snprintf(buffer, buffer_size, "<cgreen,white>Downloading\n<cgreen,white>%s", file);
    _fNotifMgrSend(buffer);

    free(buffer);
}

void _fMpClientPrepareAssets() {
    const char *dir = _fMpClientGetNewDirectory();

    if (!DirectoryExists(dir)) {
        MakeDirectory(dir);
    }
}
void _fMpClientDownloadAssets(const char **assets, unsigned int array_length) {
    struct fmp_download_context *ctx = (struct fmp_download_context *)malloc(sizeof(struct fmp_download_context));

    ctx->array_length = array_length;
    ctx->assets = assets;
    ctx->current_file_index = 0;
    ctx->connection = _fCurlConnectionCreate();

    _fMpClientDownloadFile(ctx);
}
void _fMpClientSwitchAssets() {
    ChangeDirectory(_fMpClientGetNewDirectory());
}

const char *_fMpClientGetNewDirectory() {
    return "remote-assets";
}

#endif
