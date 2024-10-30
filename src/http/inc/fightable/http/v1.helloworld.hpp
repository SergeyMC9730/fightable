#pragma once

#include <fightable/http/ApiRequest.hpp>
#include <memory>

using namespace httpserver;

namespace LevelAPI {
    namespace v1 {
        class HelloWorldRequest : public APIRequest {
        public:
            std::shared_ptr<http_response> render(const http_request&);

            HelloWorldRequest();
        };
    }
}