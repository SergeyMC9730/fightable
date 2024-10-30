#pragma once

#include <fightable/http/ApiRequest.hpp>
#include <fightable/http/remote_resource_manager.h>

struct fhttpserver {
    httpserver::webserver *ws = nullptr;
    unsigned short port = 3000;

    frrm remote_res_msg = {};
};