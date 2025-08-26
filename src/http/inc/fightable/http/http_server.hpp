
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <fightable/http/ApiRequest.hpp>
#include <fightable/http/remote_resource_manager.h>

struct fhttpserver {
    httpserver::webserver *ws = nullptr;
    unsigned short port = 3000;

    frrm remote_res_msg = {};

    void (*logger)(const char *msg);
};
