#include "fightable/http/http_server.h"
#include <fightable/http/v1.helloworld.hpp>
#ifdef TARGET_UNIX
#include <unistd.h>
#include <array>
#include <sys/types.h>
#include <dirent.h>
#endif

LevelAPI::v1::HelloWorldRequest::HelloWorldRequest() {
    this->request_name = "hello world";
    this->request_url = "/api/v1/hello";
}

std::shared_ptr<http_response> LevelAPI::v1::HelloWorldRequest::render(const http_request &req) {
    auto a = req.get_arg("mykey");

#ifdef TARGET_UNIX
    std::string dir = _fHttpGetAllowedResourceDir(_httpServer);

    std::string res = " Working directory: " + dir;

    DIR *dp = opendir(_fHttpGetAllowedResourceDir(_httpServer));
    if (!dp) {
        res += "\n Directory listing\ncannot be created";
    } else {
        dirent *ep;

        res += "\n Directory listing:\n";

        while ((ep = readdir(dp)) != nullptr) {
            res += "- " + std::string(ep->d_name) + "\n";
        }

        closedir(dp);
    }

    return generateResponse(res);
#else
    return generateResponse(a.get_flat_value());
#endif
}
