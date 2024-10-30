#include <fightable/http/http_server.h>
#include <fightable/http/http_server.hpp>
#include <fightable/http/requests.hpp>
#include <vector>
#include <string.h>
#include <arpa/inet.h>

struct fhttpserver *_fHttpServerCreate(unsigned short port, void (*logger)(const char *msg)) {
    struct fhttpserver *srv = new struct fhttpserver;

    auto params = httpserver::create_webserver()
        .port(port)
        .max_connections(128)
        .max_threads(4)
        .memory_limit(256 * 1024)
        .debug()
    ;

    srv->ws = new httpserver::webserver(params);
    
    std::vector<APIRequest *> requests = {
        new LevelAPI::v1::HelloWorldRequest(),
        new LevelAPI::v1::ResourceRequest()
    };

    for (APIRequest *request : requests) {
        request->setServer(srv);

        srv->ws->register_resource(request->request_url, request->getAsResource());
    }

    try {
        srv->ws->start(false);
    } catch (std::invalid_argument e) {
        if (logger != nullptr) {
            std::string msg = "Could not start webserver: " + std::string(e.what());
            logger(msg.c_str());
        }

        delete srv->ws;
        delete srv;

        return nullptr;
    }

    _fHttpSetAllowedResourceDir(srv, "assets");

    return srv;
}