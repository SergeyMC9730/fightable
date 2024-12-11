#pragma once

class ftcp_server_daemon;

#include <fightable/tcpsrv/message.hpp>
#include <vector>

struct ftcp_server_user {
private:
    int _descriptor;
    int _userID;

    bool _disconnect = false;

    char _username[64];
    
    ftcp_server_daemon *_daemon = nullptr;

    std::vector<ftcp_server_message> _requestedMessages;

    bool _sendMessage(const std::string &message);
public:
    ftcp_server_user();
    ftcp_server_user(int descriptor);
    ftcp_server_user(int descriptor, int userID);
    ftcp_server_user(const ftcp_server_user &ref);

    inline int getDescriptor() const { return _descriptor; }
    inline int getUserID() const { return _userID; }

    void disconnect();
    bool shouldDisconnect() const;

    void setUsername(const char *username, bool shouldDecrypt = false);
    const char *getUsername() const;

    ftcp_server_daemon *getDaemon() const;
    void setDaemon(ftcp_server_daemon *daemon);

    bool sendMessage(const std::string &message);

    std::string encryptUsername();

    std::string generateMultiMessage();
    void clearMessageQueue();
    int getMessagesRequested();

    friend ftcp_server_daemon;
};