#include <fightable/tcpsrv/daemon.hpp>
#include <fightable/tcpsrv/delegate.h>
#include <fightable/tcpsrv/daemon.h>
#include <fightable/tcpsrv/tools.hpp>


#ifdef TARGET_UNIX
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <asm-generic/socket.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#elif defined(TARGET_WIN32)
#include <windows.h>
#include <winsock.h>
#include <io.h>
#endif

#include <assert.h>
#include <fcntl.h>

#include <iostream>
#include <algorithm>

ftcp_server_daemon::ftcp_server_daemon(unsigned int port, ftcp_server_delegate *delegate) {
    _connectionBuffer.fill(0x00);

    unsigned int maxClients = 0xFFFF;
    int opt = 1;

    _maxClients = maxClients;

    int status = 0;

#ifdef TARGET_UNIX
    struct rlimit rlim;
    rlim.rlim_cur = maxClients;
    rlim.rlim_max = maxClients;

    _baseSocketOffset = _nextSocketID;

    status = setrlimit(RLIMIT_NOFILE, &rlim);
    assert(status >= 0 && "TcpServerDaemon: setrlimit: fail");
#else
    printf("TcpServerDaemon: client limit cannot be set on this platform\n");
#endif

    // _socketInfo.acceptedConnections.reserve(maxClients);

    _delegate = delegate;
    assert(_delegate != nullptr && "TcpServerDaemon: delegate is not created");

    _socketInfo.masterSocket = socket(AF_INET, SOCK_STREAM, 0);
    assert(_socketInfo.masterSocket != 0 && "TcpServerDaemon: socket: fail");

    _socketInfo.addrLen = sizeof(_socketInfo.address);

    status = setsockopt(_socketInfo.masterSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt));
    assert(status >= 0 && "TcpServerDaemon: setsockopt (SO_REUSEADDR): fail");

    struct linger ling = {
        1,
        9999
    };

    status = setsockopt(_socketInfo.masterSocket, SOL_SOCKET, SO_LINGER, (char *)&ling, sizeof(ling));
    assert(status >= 0 && "TcpServerDaemon: setsockopt (SO_LINGER): fail");

    _socketInfo.address.sin_family = AF_INET;
    _socketInfo.address.sin_addr.s_addr = INADDR_ANY;
    _socketInfo.address.sin_port = htons(port);

    status = bind(_socketInfo.masterSocket, (const struct sockaddr *)&_socketInfo.address, _socketInfo.addrLen);
    assert(status >= 0 && "TcpServerDaemon: bind: fail");

    status = listen(_socketInfo.masterSocket, 8192);
    assert(status >= 0 && "TcpServerDaemon: listen: fail");

    _baseThreadValue = std::thread([](ftcp_server_daemon *daemon) {
        daemon->_baseThread();
    }, this);
    _queueThreadValue = std::thread([](ftcp_server_daemon *daemon) {
        daemon->_queueThread();
    }, this);
    _eventThreadValue = std::thread([](ftcp_server_daemon *daemon) {
        daemon->_eventThread();
    }, this);

    _baseThreadValue.detach();
    _queueThreadValue.detach();
    _eventThreadValue.detach();

    std::cout << "TcpServerDaemon: listening on 0.0.0.0:" << port << std::endl;
}

const char *ftcp_server_daemon::_getConnectionHeader() {
    return "fightable-1";
}

void ftcp_server_daemon::_baseThread() {
    int maxSocketDescriptor;
    socklen_t addrLen = _socketInfo.addrLen;

    while (true) {
        FD_ZERO(&_socketInfo.socketDescriptors);

        FD_SET(_socketInfo.masterSocket, &_socketInfo.socketDescriptors);
        maxSocketDescriptor = _socketInfo.masterSocket;

        for (int descriptor : _socketInfo.acceptedConnections) {
            if (descriptor <= 0) {
                printf("loop 2\n");

                continue;
            }

            FD_SET(descriptor, &_socketInfo.socketDescriptors);

            if (descriptor > maxSocketDescriptor) {
                maxSocketDescriptor = descriptor;
            }

            printf("loop 4\n");
        }

        int status = select(maxSocketDescriptor + 1, &_socketInfo.socketDescriptors, NULL, NULL, NULL);
        if (status < 0) {
            continue;
        }
        else {
            printf("loop 3\n");
        }

        status = FD_ISSET(_socketInfo.masterSocket, &_socketInfo.socketDescriptors);
        if (status) {
            printf("loop 5\n");
            int accept_status = accept(_socketInfo.masterSocket, (struct sockaddr *)&_socketInfo.address, &addrLen);
            assert(accept_status >= 0 && "TcpServerDaemon: accept: fail");

            char *conAddress = inet_ntoa(_socketInfo.address.sin_addr);
            unsigned int conPort = ntohs(_socketInfo.address.sin_port);

            if (countIPAddress(conAddress) >= _maxClientsPerIP) {
                printf("loop 6\n");
                close(accept_status);
            } else {
                printf("loop 7\n");
                _ipAddresses.push_back(conAddress);

                int old_status = accept_status;
                _nextSocketID = nextFreeSocketID();

                dup2(accept_status, _nextSocketID);

                close(old_status);

                accept_status = _nextSocketID;

                _ipMap[accept_status] = conAddress;

                // fcntl(accept_status, FD_CLOEXEC, (int)1);
                std::cout << "TcpServerDaemon: new connection from " << conAddress << ":" << conPort << " (" << accept_status << ")\n";

                std::string _header = _getConnectionHeader();        
                std::string header = "b," + _header;

                // sendMessage(accept_status, header.c_str());
            
                _socketInfo.acceptedConnections.push_back(accept_status);

                _socketInfo.users[accept_status] = accept_status;
                _socketInfo.users[accept_status].setDaemon(this);

                _socketInfo.users[accept_status].sendMessage(header.c_str());
            }
        }
        else {
            printf("loop 8\n");
        }

        std::vector<int> descriptorsToRemove = {};

        for (int descriptor : _socketInfo.acceptedConnections) {
            printf("loop 9\n");
            bool shouldExist = _processDescriptor(descriptor);

            if (!shouldExist) {
                printf("loop 10\n");
                descriptorsToRemove.push_back(descriptor);
            }
        }

        if (descriptorsToRemove.size() != 0) {
            printf("loop 11\n");
            std::vector<int> new_descriptors = {};

            for (int descriptor : _socketInfo.acceptedConnections) {
                bool contains = false;

                for (int ignoredDesc : descriptorsToRemove) {
                    if (descriptor == ignoredDesc) {
                        contains = true;
                        break;
                    }
                }

                if (contains) {
                    auto &user = _socketInfo.users[descriptor];

                    removeIPAddress(_ipMap[descriptor]);

                    _delegate->processDisconnect(_delegate, &user);

                    int close_result = close(descriptor);
                    if (close_result < 0) {
                        printf("TcpServerDaemon: close: fail (%d)\n", close_result);
                    }

                    continue;
                }

                new_descriptors.push_back(descriptor);
            }

            _socketInfo.acceptedConnections = new_descriptors;

            for (int ignoredDesc : descriptorsToRemove) {
                std::cout << "TcpServerDaemon: user " << ignoredDesc << " (" << _socketInfo.users[ignoredDesc].getUserID() << ") has been disconnected\n";
            }
        }
    }
}

bool ftcp_server_daemon::sendMessage(int socket, std::vector<unsigned char> &message) {
#ifdef TARGET_UNIX
    constexpr int flags = MSG_NOSIGNAL;
#else
    constexpr int flags = 0;
#endif

    unsigned int sz = message.size();
    int data_sent = send(socket, (const char *)message.data(), sz, flags);

    return data_sent == sz;
}

#include <cstring>

bool ftcp_server_daemon::sendMessage(int socket, const char *message) {
    printf("TcpServerDaemon: sending \"%s\" for socket %d\n", message, socket);

    std::vector<unsigned char> c;
    size_t len = strlen(message);
    
    for (size_t i = 0; i < len; i++) {
        c.push_back(message[i]);
    }

    c.push_back(0);

    return sendMessage(socket, c);
}

bool ftcp_server_daemon::_processDescriptor(int desc) {
    if (FD_ISSET(desc, &_socketInfo.socketDescriptors)) {
        int valread = read(desc, _connectionBuffer.data(), _connectionBuffer.size());

        // assert(valread != -1 && "TcpServerDaemon: read: fail");

        if (valread == 0 || valread == -1) {
            return false;
        }

        _delegate->setDaemon(_delegate, this);
        
        std::vector<unsigned char> v = {};

        for (unsigned int i = 0; i < valread; i++) {
            v.push_back(_connectionBuffer[i]);
        }

        v.push_back(0);

        unsigned char *vcStr = v.data();

        std::vector<std::string> actualMessages = GenericTools::splitString((const char *)vcStr, '|');

        ftcp_server_user &user = _socketInfo.users.at(desc);

        for (std::string &m : actualMessages) {
            auto vv = GenericTools::stringToVector<unsigned char>(m);

            _delegate->processMessage(_delegate, &user, vv.data(), vv.size());
        }

        _socketInfo.users[desc] = user;

        _connectionBuffer.fill(0);

        if (user.shouldDisconnect()) {
            return false;
        }
    }

    return true;
}

unsigned int ftcp_server_daemon::getClientsConnected() {
    return _socketInfo.acceptedConnections.size();
}

bool ftcp_server_daemon::descriptorExists(int fd) {
#ifdef TARGET_UNIX
    return fcntl(fd, F_GETFD) != -1 || errno != EBADF;
#elif defined(TARGET_WIN32)
    char type = 0xFF;
    int length = 0;

    getsockopt(fd, SOL_SOCKET, SO_TYPE, &type, &length);

    return type == SOCK_STREAM && length != 0;
#else
    assert(false && "descriptorExists is not implemented for this platform")
#endif
}

unsigned int ftcp_server_daemon::countIPAddress(std::string address) {
    unsigned int count = 0;
    
    for (std::string &ip : _ipAddresses) {
        if (address == ip) {
            count++;
        }
    }

    return count;
}

void ftcp_server_daemon::removeIPAddress(std::string address) {
    printf("TcpServerDaemon: removing ip address %s (%d)\n", address.c_str(), countIPAddress(address));

    int count = (int)countIPAddress(address) - 1;
    if (count < 0) return;

    _ipAddresses.erase(std::remove(_ipAddresses.begin(), _ipAddresses.end(), address), _ipAddresses.end());

    for (int i = 0; i < count; i++) {
        _ipAddresses.push_back(address);
    }
}

int ftcp_server_daemon::nextFreeSocketID() {
    bool selected = false;

    int id = 0;

    for (int i = _baseSocketOffset; i < _maxClients; i++) {
        if (!descriptorExists(i)) {
            selected = true;
            id = i;
            break;
        }
    }

    assert(selected != false);

    return id;
}

void ftcp_server_daemon::_queueThread() {
    while (true) {
        for (auto &[k, v] : _socketInfo.users) {
            if (v.getMessagesRequested() != 0) {
                std::string msg = v.generateMultiMessage();

                bool succeded = sendMessage(v.getDescriptor(), msg.c_str());

                if (!succeded || !descriptorExists(v.getDescriptor())) {
                    v.disconnect();
                }
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }
}

void ftcp_server_daemon::requestMessageForUser(int socket, const char *message) {
    for (auto &[k, v] : _socketInfo.users) {
        if (k == socket) {
            v._sendMessage(message);

            return;
        }
    }
}

ftcp_server_delegate *ftcp_server_daemon::getDelegate() {
    return _delegate;
}

// emerge --ask --verbose --update --deep --newuse @world wine firefox neofetch htop tmux cronie chrony dosfstools f2fs-tools bluez git

void ftcp_server_daemon::sendGlobalMessage(const char *message) {
    for (auto &[k, v] : _socketInfo.users) {
        v.sendMessage(message);
    }
}

void ftcp_server_daemon::_eventThread() {
    for (;;) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

struct ftcp_server_daemon *_fTcpSrvCreate(unsigned short port, ftcp_server_delegate *delegate) {
    return new ftcp_server_daemon(port, delegate);
}
void _fTcpSrvDestroy(struct ftcp_server_daemon *daemon) {
    if (!daemon) return;
    delete daemon;
}