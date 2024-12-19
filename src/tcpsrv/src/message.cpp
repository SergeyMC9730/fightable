#include <fightable/tcpsrv/message.hpp>
#include <string.h>
#include <vector>

ftcp_server_message::ftcp_server_message(const std::string &mesg) {
    _mesg = mesg;
    generateHeader();
}

ftcp_server_message::ftcp_server_message(const std::string &mesg, std::array<char, 4> header) {
    _mesg = mesg;
    memcpy(_header, header.data(), 4);
}

ftcp_server_message::ftcp_server_message() {
    generateHeader();
}

std::array<char, 4> ftcp_server_message::getHeader() {
    return {_header[0], _header[1], _header[2], _header[3]};
}
std::string ftcp_server_message::getBaseMessage() {
    return _mesg;
}

void ftcp_server_message::generateHeader() {
    int signatureSize = 4;

    for (int i = 0; i < signatureSize; i++) {
        char c = (rand() % 255) + 1;
        if (c == '|' || c == 0) c = 'a';
        _header[i] = c;
    }
}