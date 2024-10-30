#pragma once

#ifdef __cplusplus
extern "C" {
#endif

struct fhttpserver;

struct fhttpserver *_fHttpServerCreate(unsigned short port, void (*logger)(const char *msg));
void _fHttpServerDestroy(struct fhttpserver *server);

void _fHttpSetAllowedResourceDir(struct fhttpserver *server, const char *dir);
const char *_fHttpGetAllowedResourceDir(struct fhttpserver *server);

#ifdef __cplusplus
}
#endif
