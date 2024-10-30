#include <fightable/http/v1.res.request.hpp>
#include <fightable/http/http_server.h>

#include <string>
#include <iostream>
#include <filesystem>

#include <fightable/http/HttpContentTypeImage.hpp>
#include <fightable/http/HttpContentTypeAudio.hpp>
#include <fightable/http/HttpContentTypeText.hpp>
#include <fightable/http/HttpContentTypeOctet.hpp>
#include <fightable/http/HttpContentTypeJson.hpp>

#define file_exists(cstr) (stat(cstr, &buffer) == 0)

LevelAPI::v1::ResourceRequest::ResourceRequest() {
    this->request_name = "request data from server";
    this->request_url = "/api/v1/res/request/{file}";
}

std::shared_ptr<http_response> LevelAPI::v1::ResourceRequest::render(const http_request &req) {
    fhttpserver *srv = getServer();
    if (!srv) {
        return generateResponse("Bad server configuration", 300);
    }

    auto file = std::string(_fHttpGetAllowedResourceDir(srv)) + std::string("/") + std::string(req.get_arg("file"));
    //std::cout << file << std::endl;

    if (file.find("../") != std::string::npos || 
        file.find("/")   == 0                 || 
        file.find("~")   != std::string::npos || 
        !std::filesystem::exists(file)    
    )
    {
        return generateResponse("File could not be found", 400);
    }

    if (file.ends_with(".mp3")) {
        return sendFile(std::string(file), HTTPContentTypeAudio());
    } else if (file.ends_with(".png")) {
        return sendFile(std::string(file), HTTPContentTypeImage());
    } else if (file.ends_with(".json")) {
        return sendFile(std::string(file), HTTPContentTypeJSON());
    } else if (file.ends_with(".txt")) {
        return sendFile(std::string(file), HTTPContentTypeText());
    } else {
        return sendFile(std::string(file), HTTPContentTypeOctet());
    }
}