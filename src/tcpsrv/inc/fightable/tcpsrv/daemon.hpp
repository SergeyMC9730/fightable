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
#include <ws2tcpip.h>
#include <io.h>
typedef int socklen_t;
#endif

#include <fightable/tcpsrv/user.hpp>

struct ftcp_server_delegate;

struct ftcp_server_daemon_user_context {
    std::thread thread;
    int desc;
    bool should_exit;
};

struct ftcp_server_daemon {
private:
    bool _stopThreads = false;

    std::thread _baseThreadValue;
    std::thread _queueThreadValue;
    std::thread _eventThreadValue;

    ftcp_server_delegate* _delegate;

    unsigned int _nextSocketID = 5;
    unsigned int _baseSocketOffset;

    unsigned int _maxClients;
    unsigned int _maxClientsPerIP = 4;

    std::vector<std::string> _ipAddresses;
    std::map<int, std::string> _ipMap;

    bool _ready = false;

    std::map<int, ftcp_server_daemon_user_context*> _userThreads;

    struct SocketInfo {
        int masterSocket;
        socklen_t addrLen;
        int newSocket;

        std::vector<int> acceptedConnections;
        std::vector<int> descriptorsToRemove;

        std::map<int, ftcp_server_user> users;

        struct sockaddr_in address;

        fd_set socketDescriptors;
    } _socketInfo;

    const char* _getConnectionHeader();

    void _baseThread();
    void _queueThread();
    void _eventThread();

    // tells if descriptor should exist or not
    bool _processDescriptor(int desc, std::array<char, 4096> *buffer);

    bool descriptorExists(int fd);

    unsigned int countIPAddress(std::string address);
    void removeIPAddress(std::string address);

    int closeSocket(int fd);
    void finishSocket(int fd);
public:
    ftcp_server_daemon(unsigned int port, ftcp_server_delegate* delegate);

    bool sendMessage(int socket, const std::vector<unsigned char>& message);
    bool sendMessage(int socket, const std::string& message);
    void requestMessageForUser(int socket, const std::string& message);

    unsigned int getClientsConnected();

    int nextFreeSocketID();

    ftcp_server_delegate* getDelegate();

    void sendGlobalMessage(const std::string& message);
    void sendMessageToUser(int user_id, const std::string& message);

    bool userExists(std::string username);
    bool userExists(int user_id);

    std::optional<std::reference_wrapper<ftcp_server_user>> getUser(std::string username);
    std::optional<std::reference_wrapper<ftcp_server_user>> getUser(int user_id);
    std::optional<std::reference_wrapper<ftcp_server_user>> getUser(long descriptor);

    void setMaxClientsPerIp(unsigned int amount);
    unsigned int getMaxClientsPerIp();

    const std::vector<int>& getConnections();

    static std::string getShortHash(const std::string &str);

    ~ftcp_server_daemon();

    bool ready();
};