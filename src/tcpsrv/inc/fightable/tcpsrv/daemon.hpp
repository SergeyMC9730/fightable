#pragma once

#include <array>
#include <thread>
#include <vector>
#include <map>
#include <optional>
#ifdef TARGET_UNIX
#include <netinet/in.h>
#elif defined(TARGET_WIN32)
#include <winsock2.h>
typedef int socklen_t;
#endif

#include <fightable/tcpsrv/user.hpp>

struct ftcp_server_delegate;

struct ftcp_server_daemon {
private:
    std::array<char, 1024> _connectionBuffer;

    std::thread _baseThreadValue;
    std::thread _queueThreadValue;
    std::thread _eventThreadValue;

    ftcp_server_delegate *_delegate;

    unsigned int _nextSocketID = 5;
    unsigned int _baseSocketOffset;

    unsigned int _maxClients;
    unsigned int _maxClientsPerIP = 3;

    std::vector<std::string> _ipAddresses;
    std::map<int, std::string> _ipMap;

    struct SocketInfo {
        int masterSocket;
        socklen_t addrLen;
        int newSocket;

        std::vector<int> acceptedConnections;
        std::map<int, ftcp_server_user> users;

        struct sockaddr_in address;

        fd_set socketDescriptors;
    } _socketInfo;

    const char * _getConnectionHeader();

    void _baseThread();
    void _queueThread();
    void _eventThread();

    // tells if descriptor should exist or not
    bool _processDescriptor(int desc);

    bool descriptorExists(int fd);
    
    unsigned int countIPAddress(std::string address);
    void removeIPAddress(std::string address);
public:
    ftcp_server_daemon(unsigned int port, ftcp_server_delegate *delegate = nullptr);

    bool sendMessage(int socket, std::vector<unsigned char> &message);
    bool sendMessage(int socket, const char *message);
    
    void requestMessageForUser(int socket, const char *message);

    unsigned int getClientsConnected();

    int nextFreeSocketID();

    ftcp_server_delegate *getDelegate();
    
    void sendGlobalMessage(const char *message);
    void sendMessageToUser(int user_id, const char *message);

    bool userExists(std::string username);
    bool userExists(int user_id);

    ftcp_server_user &getUser(std::string username);
    ftcp_server_user &getUser(int user_id);
};