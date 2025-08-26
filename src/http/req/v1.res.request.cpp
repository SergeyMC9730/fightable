
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/http/v1.res.request.hpp>
#include <fightable/http/http_server.h>
#include <fightable/http/http_server.hpp>

#include <string>

#include <fightable/http/HttpContentTypeImage.hpp>
#include <fightable/http/HttpContentTypeAudio.hpp>
#include <fightable/http/HttpContentTypeText.hpp>
#include <fightable/http/HttpContentTypeOctet.hpp>
#include <fightable/http/HttpContentTypeJson.hpp>

#define file_exists(cstr) (stat(cstr, &buffer) == 0)

bool ends_with(const std::string &str, const std::string &substr) {
    if (substr.size() > str.size()) return false;
    return str.compare(str.size() - substr.size(), substr.size(), substr) == 0;
}

LevelAPI::v1::ResourceRequest::ResourceRequest() {
    this->request_name = "request data from server";
    this->request_url = "/api/v1/res/request/{file}";
}

std::shared_ptr<http_response> LevelAPI::v1::ResourceRequest::render(const http_request &req) {
    fhttpserver *srv = getServer();
    if (!srv) {
        return generateResponse("Bad server configuration", 300);
    }

    std::string filename = std::string(req.get_arg("file"));
    //std::cout << file << std::endl;

    if (filename.find("../") != std::string::npos ||
        filename.find("/")   == 0                 ||
        filename.find("~")   != std::string::npos
    )
    {
        return generateResponse("Invalid filename", 400);
    }

    auto file = std::string(_fHttpGetAllowedResourceDir(srv)) + std::string("/") + filename;

    std::string msg = "fhttpserver: accessing " + file;
    srv->logger(msg.c_str());

    if (ends_with(file, ".mp3")) {
        return sendFile(std::string(file), HTTPContentTypeAudio());
    } else if (ends_with(file, ".png")) {
        return sendFile(std::string(file), HTTPContentTypeImage());
    } else if (ends_with(file, ".json")) {
        return sendFile(std::string(file), HTTPContentTypeJSON());
    } else if (ends_with(file, ".txt")) {
        return sendFile(std::string(file), HTTPContentTypeText());
    } else {
        return sendFile(std::string(file), HTTPContentTypeOctet());
    }
}
