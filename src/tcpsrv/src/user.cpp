#include <fightable/tcpsrv/message.hpp>
#include <fightable/tcpsrv/user.hpp>
#include <stdlib.h>
#include <cstring>
#include <fightable/tcpsrv/daemon.hpp>
#include <base64.hpp>
#include <fightable/tcpsrv/user.h>
#include <fightable/shared_config.h>

ftcp_server_user::ftcp_server_user(int d) {
    srand(time(0));
    _descriptor = d;
    _userID = rand() % 0xFFFF;

    setUsername("?");
}
ftcp_server_user::ftcp_server_user(const ftcp_server_user &ref) {
    srand(time(0));
    _descriptor = ref.getDescriptor();
    _userID = ref.getUserID();
    setUsername(ref.getUsername());
    _disconnect = ref.shouldDisconnect();
    _daemon = ref.getDaemon();
}
ftcp_server_user::ftcp_server_user() {
    srand(time(0));
    _userID = rand() % 0xFFFF;
    setUsername("?");
}

void ftcp_server_user::disconnect() {
    _disconnect = true;
}
bool ftcp_server_user::shouldDisconnect() const {
    return _disconnect;
}

ftcp_server_user::ftcp_server_user(int descriptor, int userID) {
    srand(time(0));
    _descriptor = descriptor;
    _userID = userID;
    memset(_username, 0, 64);
}

void ftcp_server_user::setUsername(const char *username, bool shouldDecrypt) {
    if (shouldDecrypt) {
        std::string new_str = base64::from_base64(username);

        setUsername(new_str.c_str(), false);

        return;
    }

    memset(_username, 0, 64);
    strncpy(_username, username, 64);
    _username[63] = 0;
}
const char *ftcp_server_user::getUsername() const {
    return _username;
}

ftcp_server_daemon *ftcp_server_user::getDaemon() const {
    return _daemon;
}
void ftcp_server_user::setDaemon(ftcp_server_daemon *daemon) {
    _daemon = daemon;
}

bool ftcp_server_user::_sendMessage(const std::string &message) {
    _requestedMessages.push_back(message);

    return true;
}

bool ftcp_server_user::sendMessage(const std::string &message) {
    if (FIGHTABLE_OUTPUT_ONLY_WARNINGS) printf("ftcp_server_daemon(u): requesting message \"%s\" for %d\n", message.c_str(), getUserID());
    getDaemon()->requestMessageForUser(getDescriptor(), message);

    return true;
}

std::string ftcp_server_user::encryptUsername() {
    std::string encoded_str = base64::to_base64(_username);

    return encoded_str;
}

std::string ftcp_server_user::generateMultiMessage() {
    if (_requestedMessages.empty()) return "";

    std::string gen = "";

    for (ftcp_server_message &tcpMsg : _requestedMessages) {
        std::string msg = tcpMsg;

        gen += msg + "|";
    }

    gen.pop_back();
    
    std::string hash = ftcp_server_daemon::getShortHash(gen) + gen;

    return hash;
}
void ftcp_server_user::clearMessageQueue() {
    _requestedMessages.clear();
}

int ftcp_server_user::getMessagesRequested() {
    return _requestedMessages.size();
}

void _fTcpSrvUserSendMessage(struct ftcp_server_user *user, const char *message) {
    if (!user) return;
    
    std::string msg = message;

    user->sendMessage(msg);
}
const char *ftcp_server_user::encryptUsername2() {
    _base64username = encryptUsername();

    return _base64username.c_str();
}

const char *_fTcpSrvUserGetName(struct ftcp_server_user *user) {
    if (!user) return NULL;

    return user->getUsername();
}
int _fTcpSrvUserGetId(struct ftcp_server_user *user) {
    if (!user) return -1;

    return user->getUserID();
}
int _fTcpSrvUserGetDescriptor(struct ftcp_server_user *user) {
    if (!user) return -1;

    return user->getDescriptor();
}

void _fTcpSrvUserDisconnect(struct ftcp_server_user *user) {
    if (!user) return;

    user->disconnect();
}

void _fTcpSrvUserSetName(struct ftcp_server_user *user, const char *username) {
    if (!user) return;

    user->setUsername(username, false);
}
void _fTcpSrvUserSetNameEncrypted(struct ftcp_server_user *user, const char *username) {
    if (!user) return;

    user->setUsername(username, true);
}

const char* _fTcpSrvUserGetNameEncrypted(struct ftcp_server_user* user) {
    if (!user) return nullptr;

    return user->encryptUsername2();
}