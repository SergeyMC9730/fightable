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
    std::string res = " Working directory: ";

    {
        std::array<char, 1024> buffer;
        getcwd(buffer.data(), buffer.size());
        
        res += std::string(buffer.data());
    }

    DIR *dp = opendir("./");
    if (!dp) {
        res += "\n Directory listing\ncannot be created";
    } else {
        dirent64 *ep;

        res += "\n Directory listing:";

        while ((ep = readdir64(dp)) != nullptr) {
            res += "- " + std::string(ep->d_name);
        }

        closedir(dp);
    }

    return generateResponse(res);
#else
    return generateResponse(a.get_flat_value());
#endif
}