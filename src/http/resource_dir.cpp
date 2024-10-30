#include <fightable/http/http_server.h>
#include <fightable/http/http_server.hpp>

void _fHttpSetAllowedResourceDir(struct fhttpserver *server, const char *dir) {
    if (!server) return;

    server->remote_res_msg.allowed_resource_dir = dir;
}
const char *_fHttpGetAllowedResourceDir(struct fhttpserver *server) {
    if (!server) return nullptr;

    return server->remote_res_msg.allowed_resource_dir;
}