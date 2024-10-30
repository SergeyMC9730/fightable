#pragma once

#include <fightable/http/ApiRequest.hpp>

struct fhttpserver {
    httpserver::webserver *ws;
    unsigned short port;
};