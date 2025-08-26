
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/http/http_server.h>
#include <fightable/http/http_server.hpp>
#include <fightable/http/requests.hpp>
#include <vector>

void _fHttpServerDefaultLogger(const char *msg) {
    printf("[fhttpserver] %s\n", msg);
}

struct fhttpserver *_fHttpServerCreate(unsigned short port, void (*logger)(const char *msg)) {
    if (logger == nullptr) logger = _fHttpServerDefaultLogger;

    struct fhttpserver *srv = new struct fhttpserver;
    srv->logger = logger;

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

        std::string msg = "fhttpserver: registered resource " + std::string(request->request_url);
        logger(msg.c_str());
    }

    try {
        srv->ws->start(false);
        logger("fhttpserver: started webserver");
    } catch (std::invalid_argument e) {
        std::string msg = "fhttpserver: could not start webserver: " + std::string(e.what());
        logger(msg.c_str());

        delete srv->ws;
        delete srv;

        for (auto request : requests) {
            delete request;
        }

        return nullptr;
    }

    _fHttpSetAllowedResourceDir(srv, "assets");

    std::string msg = "fhttpserver: allowed resource dir: " + std::string(_fHttpGetAllowedResourceDir(srv));
    logger(msg.c_str());

    return srv;
}
