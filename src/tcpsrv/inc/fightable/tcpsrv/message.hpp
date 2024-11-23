#pragma once

#include <string>
#include <array>

struct ftcp_server_message {
private:
    std::string _mesg = "";
    char _header[5] = {0,0,0,0,0};

    void generateHeader();
public:
    ftcp_server_message(const std::string &mesg);
    ftcp_server_message(const std::string &mesg, std::array<char, 4> header);
    ftcp_server_message();

    std::array<char, 4> getHeader();
    std::string getBaseMessage();

    operator std::string() {
        std::string hStr = _header;
        
        return hStr + getBaseMessage();
    }
};