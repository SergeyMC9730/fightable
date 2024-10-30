#include <fightable/http/http_server.h>
#include <fightable/http/http_server.hpp>
#include <fightable/http/requests.hpp>
#include <vector>

void _fHttpServerDestroy(struct fhttpserver *server) {
    server->ws->stop();
    
    delete server->ws;
    delete server;
}