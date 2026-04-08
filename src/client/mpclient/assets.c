
//          Sergei Baigerov 2024 - 2026.
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
#include <fightable/translation.h>

struct fmp_download_context {
    const char **assets;
    unsigned int array_length;
    unsigned int current_file_index;
    struct fcurl_connection *connection;
    char *current_url;
    void (*on_download_success)(void*);
    void *on_download_success_ctx;

    struct fnotif_mgr_entry *current_notif;
    char *notif_content;
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

void _fMpClientUpdateNotif(void *_ctx) {
    struct fmp_download_context *ctx = (struct fmp_download_context *)_ctx;

    if (!ctx->current_notif || !ctx->connection->current_operation) {
        return;
    }

    ctx->current_notif->popup->complete_progress = ctx->connection->current_operation->result->download_progress;

    _fScheduleOverlayFunc((renderer_event_t){_fMpClientUpdateNotif, _ctx});
}
void _fMpClientSendNotif(void *_ctx) {
    struct fmp_download_context *ctx = (struct fmp_download_context *)_ctx;

    int id = _fNotifMgrSendWithTime(ctx->notif_content, 0);
    ctx->current_notif = _fNotifMgrGetEntryById(id);

    free(ctx->notif_content);
    ctx->notif_content = NULL;

    _fScheduleOverlayFunc((renderer_event_t){_fMpClientUpdateNotif, _ctx});
}

void _fMpClientDownloadFile(struct fmp_download_context *ctx) {
    if (!ctx || ctx->current_file_index >= ctx->array_length) {
        TraceLog(LOG_ERROR, "Cannot download next file");

        if (ctx) {
            if (ctx->on_download_success) {
                ctx->on_download_success(ctx->on_download_success_ctx);
            }

            if (ctx->current_notif) {
                ctx->current_notif->popup->complete_progress = 1.f;
                ctx->current_notif->max_time = 1.f;
                ctx->current_notif->time = 1.f;
                ctx->current_notif = NULL;
            }

            renderer_event_t e;
            e.callback = _fMpClientRequestCurlDestruction;
            e.user = ctx->connection;
            _fScheduleOverlayFunc(e);

            free(ctx);
        }
        return;
    }

    if (ctx->current_notif) {
        ctx->current_notif->popup->complete_progress = 1.f;
        ctx->current_notif->max_time = 1.f;
        ctx->current_notif->time = 1.f;
        ctx->current_notif = NULL;
    }

    const char *dir = _fMpClientGetNewDirectory();
    const char *file = ctx->assets[ctx->current_file_index];

    unsigned int buffer_size = 1024;
    char *buffer = (char *)malloc(buffer_size);

    const char *storage_path = _fStorageGetWritable();

    snprintf(buffer, buffer_size, "%s/%s", storage_path, dir);
    MakeDirectory(buffer);

    snprintf(buffer, buffer_size, "%s/%s/%s", storage_path, dir, file);

    TraceLog(LOG_INFO, "Downloading %s", buffer);

    FILE *f = fopen(buffer, "wb");

    char *url_buffer = (char *)malloc(512);
    memset(url_buffer, 0, 512);

    snprintf(url_buffer, 511, "http://%s:%d/api/v1/res/request/%s", __state.mp_client_ip, __state.mp_client_http_port, file);

    if (!ctx->connection) {
        ctx->connection = _fCurlConnectionCreate();
    }
    struct fcurl_con_settings *settings = _fCurlConnectionDownloadPage(ctx->connection, url_buffer, f, 1, _fMpClientOnDownloadedFile);
    settings->custom_data = ctx;
    ctx->current_file_index++;

    ctx->notif_content = (char *)MemAlloc(512);
    snprintf(ctx->notif_content, 512, _fTranslationGetString("notification.mpc.download"), file);

    _fScheduleOverlayFunc((renderer_event_t){_fMpClientSendNotif, ctx});

    free(buffer);
}

void _fMpClientPrepareAssets() {
    const char *dir = _fMpClientGetNewDirectory();

    if (!DirectoryExists(dir)) {
        MakeDirectory(dir);
    }
}
void _fMpClientDownloadAssets(const char **assets, unsigned int array_length, void (*on_download_success)(void*), void *on_download_success_ctx) {
    struct fmp_download_context *ctx = (struct fmp_download_context *)MemAlloc(sizeof(struct fmp_download_context));

    ctx->array_length = array_length;
    ctx->assets = assets;
    ctx->current_file_index = 0;
    ctx->connection = _fCurlConnectionCreate();
    ctx->on_download_success = on_download_success;
    ctx->on_download_success_ctx = on_download_success_ctx;

    _fMpClientDownloadFile(ctx);
}
void _fMpClientSwitchAssets() {
    ChangeDirectory(_fMpClientGetNewDirectory());
}

const char *_fMpClientGetNewDirectory() {
    return "remote-assets";
}

#endif
