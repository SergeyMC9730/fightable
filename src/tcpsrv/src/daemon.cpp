#include <fightable/tcpsrv/daemon.hpp>
#include <fightable/tcpsrv/delegate.h>
#include <fightable/tcpsrv/daemon.h>
#include <fightable/generic_tools.hpp>
#include <fightable/tcpsrv/user.h>
#include <fightable/sockcompat.h>
#include <fightable/shared_config.h>
extern "C" {
    #include <md5.h>
}

#ifdef TARGET_UNIX
#include <fightable/time.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/socket.h>
// #include <asm-generic/socket.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#elif defined(TARGET_WIN32)
#undef NDEBUG
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock.h>
#include <io.h>
#endif

#if defined(TARGET_UNIX) && !defined(TARGET_LINUX)
#include <sys/socket.h>
#elif defined(TARGET_UNIX) && defined(TARGET_LINUX)
#include <asm-generic/socket.h>
#endif

#include <assert.h>
#include <fcntl.h>

#include <iostream>
#include <algorithm>

ftcp_server_daemon::ftcp_server_daemon(unsigned int port, ftcp_server_delegate* delegate) {
    unsigned int maxClients = 0xFFFF;
    int opt = 1;

    _maxClients = maxClients;

    int status = 0;

    _baseSocketOffset = _nextSocketID;

#ifdef TARGET_UNIX
    struct rlimit rlim;
    rlim.rlim_cur = maxClients;
    rlim.rlim_max = maxClients;

    status = setrlimit(RLIMIT_NOFILE, &rlim);
    assert(status >= 0 && "ftcp_server_daemon: setrlimit: fail");
#else
    printf("ftcp_server_daemon: client limit cannot be set on this platform\n");
#endif

    // _socketInfo.acceptedConnections.reserve(maxClients);

    _delegate = delegate;
    assert(_delegate != nullptr && "ftcp_server_daemon: delegate is not created");

#ifdef TARGET_WIN32
    WSADATA wsaData;

    status = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (FIGHTABLE_OUTPUT_ONLY_WARNINGS) printf("ftcp_server_daemon: WSAStartup returned %d\n", status);
    assert(status == 0 && "ftcp_server_daemon: WSAStartup: fail");
#endif

    _socketInfo.masterSocket = socket(AF_INET, SOCK_STREAM, 0);
    assert(_socketInfo.masterSocket != 0 && "ftcp_server_daemon: socket: fail");

    _socketInfo.addrLen = sizeof(_socketInfo.address);

    status = setsockopt(_socketInfo.masterSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));
    assert(status >= 0 && "ftcp_server_daemon: setsockopt (SO_REUSEADDR): fail");

    struct linger ling = {
        1,
        9999
    };

    status = setsockopt(_socketInfo.masterSocket, SOL_SOCKET, SO_LINGER, (char*)&ling, sizeof(ling));
    assert(status >= 0 && "ftcp_server_daemon: setsockopt (SO_LINGER): fail");

    _socketInfo.address.sin_family = AF_INET;
    _socketInfo.address.sin_addr.s_addr = INADDR_ANY;
    _socketInfo.address.sin_port = htons(port);

    status = bind(_socketInfo.masterSocket, (const struct sockaddr*)&_socketInfo.address, _socketInfo.addrLen);
    if (status < 0) {
        printf("ftcp_server_daemon: bind: fail (%d)\n", status);
        return;
    }

    status = listen(_socketInfo.masterSocket, 8192);
    if (status < 0) {
        printf("ftcp_server_daemon: listen: fail (%d)\n", status);
        return;
    }

    _baseThreadValue = std::thread([](ftcp_server_daemon* daemon) {
        daemon->_baseThread();
        }, this);
    _queueThreadValue = std::thread([](ftcp_server_daemon* daemon) {
        daemon->_queueThread();
        }, this);
    _eventThreadValue = std::thread([](ftcp_server_daemon* daemon) {
        daemon->_eventThread();
        }, this);

    std::cout << "ftcp_server_daemon: listening on 0.0.0.0:" << port << std::endl;

    _ready = true;
}

const char* ftcp_server_daemon::_getConnectionHeader() {
    return "fightable-1";
}

void ftcp_server_daemon::_baseThread() {
    int maxSocketDescriptor;
    socklen_t addrLen = _socketInfo.addrLen;

    while (!_stopThreads) {
        FD_ZERO(&_socketInfo.socketDescriptors);

        FD_SET(_socketInfo.masterSocket, &_socketInfo.socketDescriptors);
        maxSocketDescriptor = _socketInfo.masterSocket;

        for (int descriptor : _socketInfo.acceptedConnections) {
            if (descriptor <= 0) {
                // printf("loop 2\n");

                continue;
            }

            FD_SET(descriptor, &_socketInfo.socketDescriptors);

            if (descriptor > maxSocketDescriptor) {
                maxSocketDescriptor = descriptor;
            }

            // printf("loop 4\n");
        }

        int status = select(maxSocketDescriptor + 1, &_socketInfo.socketDescriptors, NULL, NULL, NULL);
        if (status < 0) {
            continue;
        }
        else {
            // printf("loop 3\n");
        }

        status = FD_ISSET(_socketInfo.masterSocket, &_socketInfo.socketDescriptors);
        if (status) {
            // printf("loop 5\n");
            int accept_status = accept(_socketInfo.masterSocket, (struct sockaddr*)&_socketInfo.address, &addrLen);
           /* assert(accept_status >= 0 && "ftcp_server_daemon: accept: fail");*/
            if (accept_status < 0) {
                printf("ftcp_server_daemon: accept: fail (%d)\n", accept_status);
                return;
            }

            char* conAddress = inet_ntoa(_socketInfo.address.sin_addr);
            unsigned int conPort = ntohs(_socketInfo.address.sin_port);

            if (countIPAddress(conAddress) >= _maxClientsPerIP) {
                // printf("loop 6\n");
                printf("ftcp_server_daemon: accept: user %s connected too much times\n", conAddress);
                closeSocket(accept_status);
            }
            else {
                if (FIGHTABLE_OUTPUT_ONLY_WARNINGS) printf("ftcp_server_daemon: accept: attempting to configure user %s\n", conAddress);

                _ipAddresses.push_back(conAddress);
                int old_status = accept_status;

#ifndef TARGET_UNIX
                printf("ftcp_server_daemon: socket %d cannot be duplicated on this platform\n", old_status);

                // while you  can use  _open_osfhandle on  NT platforms 
                // this function doesn't guarantee it will return valid 
                // handle
#else
                _nextSocketID = nextFreeSocketID();
                NPD_DUP2(accept_status, _nextSocketID);
                closeSocket(old_status);
                accept_status = _nextSocketID;
#endif

#ifdef TARGET_WIN32
                FD_SET(accept_status, &_socketInfo.socketDescriptors);
#endif

                _ipMap[accept_status] = conAddress;

                // fcntl(accept_status, FD_CLOEXEC, (int)1);
                std::cout << "ftcp_server_daemon: new connection from " << conAddress << ":" << conPort << " (" << accept_status << ")\n";

                std::string _header = _getConnectionHeader();
                std::string header = "b," + _header;

                // sendMessage(accept_status, header.c_str());

                _socketInfo.acceptedConnections.push_back(accept_status);

                _socketInfo.users[accept_status] = accept_status;
                _socketInfo.users[accept_status].setDaemon(this);

                _socketInfo.users[accept_status].sendMessage(header);

                if (_delegate->processConnect) {
                    _delegate->processConnect(_delegate, std::addressof(_socketInfo.users.at(accept_status)));
                }

                if (FIGHTABLE_OUTPUT_ONLY_WARNINGS) printf("ftcp_server_daemon: opening new thread for this connection\n");

                ftcp_server_daemon_user_context* ctx = new ftcp_server_daemon_user_context;
                ctx->should_exit = false;
                ctx->desc = accept_status;
                ctx->thread = std::thread([this, ctx]() {
                    auto buffer = new std::array<char, 4096>();
                    buffer->fill(0);

                    while (!ctx->should_exit) {
                        if (!_processDescriptor(ctx->desc, buffer)) {
                            _socketInfo.descriptorsToRemove.push_back(ctx->desc);
#ifdef TARGET_UNIX
                            _fSleep(10);
#endif
                            return;
                        }
                    }

                    delete buffer;
                });

                _userThreads[accept_status] = ctx;
            }
        }
        else {
            // printf("loop 8\n");
        }

        //std::vector<int> descriptorsToRemove = {};

        //for (int descriptor : _socketInfo.acceptedConnections) {
        //    // printf("loop 9\n");
        //    bool shouldExist = _processDescriptor(descriptor);

        //    if (!shouldExist) {
        //        // printf("loop 10\n");
        //        descriptorsToRemove.push_back(descriptor);
        //    }
        //}

        if (_socketInfo.descriptorsToRemove.size() != 0) {
            // printf("loop 11\n");
            std::vector<int> new_descriptors = {};

            for (int descriptor : _socketInfo.acceptedConnections) {
                bool contains = false;

                for (int ignoredDesc : _socketInfo.descriptorsToRemove) {
                    if (descriptor == ignoredDesc) {
                        contains = true;
                        break;
                    }
                }

                if (contains) {
                    auto& user = _socketInfo.users[descriptor];

                    removeIPAddress(_ipMap[descriptor]);
                    _delegate->processDisconnect(_delegate, &user);
                    user.clearMessageQueue();

                    int close_result = closeSocket(descriptor);
                    if (close_result < 0) {
                        printf("ftcp_server_daemon: ::closeSocket: fail (%d)\n", close_result);
                    }

                    continue;
                }

                new_descriptors.push_back(descriptor);
            }

            _socketInfo.acceptedConnections = new_descriptors;

            for (int ignoredDesc : _socketInfo.descriptorsToRemove) {
                finishSocket(ignoredDesc);

                std::cout << "ftcp_server_daemon: user " << ignoredDesc << " (" << _socketInfo.users[ignoredDesc].getUserID() << ") has been disconnected\n";
                
                _socketInfo.users.erase(ignoredDesc);
            }

            _socketInfo.descriptorsToRemove.clear();
        }
    }
}

bool ftcp_server_daemon::sendMessage(int socket, const std::vector<unsigned char>& message) {
#ifdef TARGET_UNIX
    constexpr int flags = MSG_NOSIGNAL;
#else
    constexpr int flags = 0;
#endif

    unsigned int sz = message.size();
    int data_sent = send(socket, (const char*)message.data(), sz, flags);

#ifdef TARGET_WIN32
    if (FIGHTABLE_OUTPUT_ONLY_WARNINGS) printf("ftcp_server_daemon: send: tried to send %d bytes. sent %d bytes total\n", sz, data_sent);
#endif

    return data_sent == sz;
}

#include <cstring>

bool ftcp_server_daemon::sendMessage(int socket, const std::string& message) {
    if (FIGHTABLE_OUTPUT_ONLY_WARNINGS) printf("ftcp_server_daemon: sending \"%s\" for socket %d\n", message.c_str() + 4, socket);

    std::vector<unsigned char> c;
    for (char cc : message) {
        c.push_back(cc);
    }

    c.push_back(0);

    return sendMessage(socket, c);
}

#include <md5.h>

bool ftcp_server_daemon::_processDescriptor(int desc, std::array<char, 4096>* buffer) {
    // if (FD_ISSET(desc, &_socketInfo.socketDescriptors)) {
#ifndef TARGET_WIN32
        int valread = NPD_READ(desc, buffer->data(), buffer->size() - 1);
#else 
        int valread = recv(desc, buffer->data(), buffer->size() - 1, 0);
#endif

        // assert(valread != -1 && "ftcp_server_daemon: read: fail");

        if (valread <= 0) {
            printf("ftcp_server_daemon: read: fail (%d)\n", valread);
#ifdef TARGET_WIN32
            printf("                    WSAGetLastError: %d\n", WSAGetLastError());
#endif
	        return false;
        }

        if (valread < 12) {
            printf("ftcp_server_daemon: invalid header received\n");
            return true;
        }

        ((char*)buffer->data())[valread] = 0;

        _delegate->daemon = this;

        const char* vcStr = buffer->data() + 12;
        std::string vStr = vcStr;

        std::string orig_hash{ vcStr - 12, 12 };
        std::string hash = getShortHash(vStr);

        if (orig_hash != hash) {
            printf("ftcp_server_daemon: invalid hash. %s(o) != %s(e)\n", orig_hash.c_str(), hash.c_str());
            return true;
        }

        std::vector<std::string> actualMessages = GenericTools::splitString(vcStr, '|');

        ftcp_server_user& user = _socketInfo.users.at(desc);

        for (std::string& m : actualMessages) {
            auto vv = GenericTools::stringToVector<unsigned char>(m);

            if (vv.size() != 0 && vv.data()[0] == '$') {
                user.clearMessageQueue();
            }

            vv.push_back(0);

            _delegate->processMessage(_delegate, std::addressof(user), vv.data(), vv.size());
        }

        _socketInfo.users[desc] = user;

        if (user.shouldDisconnect()) {
            printf("ftcp_server_daemon: read: force disconnect (%d bytes read)\n", valread);
            return false;
        }

        return true;
    // }
    // else {
        // printf("ftcp_server_daemon: FD_ISSET: descriptor %d not set\n", desc);
        // return false;
    // }
}

unsigned int ftcp_server_daemon::getClientsConnected() {
    return _socketInfo.acceptedConnections.size();
}

bool ftcp_server_daemon::descriptorExists(int fd) {
#ifdef TARGET_UNIX
    return fcntl(fd, F_GETFD) != -1 || errno != EBADF;
#elif defined(TARGET_WIN32)
    int r = recv(fd, NULL, 0, 0);
    if (r == SOCKET_ERROR && WSAGetLastError() == WSAECONNRESET) {
        return true;
    }
    return false;
#else
    assert(false && "descriptorExists is not implemented for this platform")
#endif
}

unsigned int ftcp_server_daemon::countIPAddress(std::string address) {
    unsigned int count = 0;

    for (std::string& ip : _ipAddresses) {
        if (address == ip) {
            count++;
        }
    }

    return count;
}

void ftcp_server_daemon::removeIPAddress(std::string address) {
    if (FIGHTABLE_OUTPUT_ONLY_WARNINGS) printf("ftcp_server_daemon: removing ip address %s (%d)\n", address.c_str(), countIPAddress(address));

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
        if (FIGHTABLE_OUTPUT_ONLY_WARNINGS) printf("ftcp_server_daemon: nextFreeSocketID: trying %d\n", i);

        if (!descriptorExists(i)) {
            if (FIGHTABLE_OUTPUT_ONLY_WARNINGS) printf("ftcp_server_daemon: nextFreeSocketID: %d is available for use\n", i);
            selected = true;
            id = i;
            break;
        }
    }

    assert(selected != false);

    return id;
}

void ftcp_server_daemon::_queueThread() {
    while (!_stopThreads) {
        for (auto& [k, v] : _socketInfo.users) {
            if (v.getMessagesRequested() != 0) {
                std::string msg = v.generateMultiMessage();

                bool succeded = sendMessage(v.getDescriptor(), msg.c_str());
#ifndef TARGET_WIN32
                bool desc_exists = descriptorExists(v.getDescriptor());
#else 
                bool desc_exists = true;
#endif

                if (!succeded || !desc_exists) {
#ifdef TARGET_WIN32
                    printf("ftcp_server_daemon: _queueThread: one of conditions failed: %d,%d. disconnecting user\n", succeded, desc_exists);
#endif
                    v.disconnect();
                }
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }
}

void ftcp_server_daemon::requestMessageForUser(int socket, const std::string& message) {
    for (auto& [k, v] : _socketInfo.users) {
        if (k == socket) {
            v._sendMessage(message);

            return;
        }
    }
}

ftcp_server_delegate* ftcp_server_daemon::getDelegate() {
    return _delegate;
}

void ftcp_server_daemon::sendGlobalMessage(const std::string& message) {
    for (auto& [k, v] : _socketInfo.users) {
        v.sendMessage(message);
    }
}

void ftcp_server_daemon::_eventThread() {
    while (!_stopThreads) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

struct ftcp_server_daemon* _fTcpSrvCreate(unsigned short port, ftcp_server_delegate* delegate) {
    return new ftcp_server_daemon(port, delegate);
}
void _fTcpSrvDestroy(struct ftcp_server_daemon* daemon) {
    if (!daemon) return;
    delete daemon;
}

void ftcp_server_daemon::sendMessageToUser(int user_id, const std::string& message) {
    auto u = getUser(user_id);
    if (!u.has_value()) return;

    if (FIGHTABLE_OUTPUT_ONLY_WARNINGS) printf("ftcp_server_daemon(d): requesting message \"%s\" for %d\n", message.c_str(), u.value().get().getUserID());

    u.value().get().sendMessage(message);
}

bool ftcp_server_daemon::userExists(std::string username) {
    for (auto& [k, v] : _socketInfo.users) {
        if (v.getUsername() == username) return true;
    }

    return false;
}
bool ftcp_server_daemon::userExists(int user_id) {
    for (auto& [k, v] : _socketInfo.users) {
        if (v.getUserID() == user_id) return true;
    }

    return false;
}

std::optional<std::reference_wrapper<ftcp_server_user>> ftcp_server_daemon::getUser(std::string username) {
    for (auto& [k, v] : _socketInfo.users) {
        if (v.getUsername() == username) return v;
    }

    return std::nullopt;
}
std::optional<std::reference_wrapper<ftcp_server_user>> ftcp_server_daemon::getUser(int user_id) {
    for (auto& [k, v] : _socketInfo.users) {
        if (v.getUserID() == user_id) return v;
    }

    return std::nullopt;
}
std::optional<std::reference_wrapper<ftcp_server_user>> ftcp_server_daemon::getUser(long descriptor) {
    for (auto& [k, v] : _socketInfo.users) {
        if (v.getDescriptor() == descriptor) return v;
    }

    return std::nullopt;
}

struct ftcp_server_user* _fTcpSrvGetUserByName(struct ftcp_server_daemon* daemon, const char* username) {
    if (!daemon) return NULL;

    auto user = daemon->getUser(username);
    if (!user.has_value()) return NULL;

    return std::addressof(user.value().get());
}
struct ftcp_server_user* _fTcpSrvGetUserById(struct ftcp_server_daemon* daemon, int user_id) {
    if (!daemon) return NULL;

    auto user = daemon->getUser(user_id);
    if (!user.has_value()) return NULL;

    return std::addressof(user.value().get());
}

void _fTcpSrvSendGlobalMsg(struct ftcp_server_daemon* daemon, const char* message) {
    if (!daemon) return;

    std::string msg = message;
    daemon->sendGlobalMessage(msg);
}

unsigned int _fTcpSrvGetConnectedUsers(struct ftcp_server_daemon* daemon) {
    if (!daemon) return 0;

    return daemon->getClientsConnected();
}

int ftcp_server_daemon::closeSocket(int fd) {
    printf("ftcp_server_daemon: disconnecting socket %d\n", fd);

#ifndef TARGET_WIN32
    return NPD_CLOSE(fd);
#else
    return closesocket(fd);
#endif
}

void ftcp_server_daemon::setMaxClientsPerIp(unsigned int amount) {
    _maxClientsPerIP = amount;
}
unsigned int ftcp_server_daemon::getMaxClientsPerIp() {
    return _maxClientsPerIP;
}

void _fTcpSrvSetMaxClientsPerIp(struct ftcp_server_daemon* daemon, unsigned int amount) {
    if (!daemon) return;

    daemon->setMaxClientsPerIp(amount);
}
unsigned int _fTcpSrvGetMaxClientsPerIp(struct ftcp_server_daemon* daemon) {
    if (!daemon) return 0;

    return daemon->getMaxClientsPerIp();
}

struct ftcp_server_user* _fTcpSrvGetUserByIndex(struct ftcp_server_daemon* daemon, unsigned int idx) {
    if (!daemon || _fTcpSrvGetConnectedUsers(daemon) <= idx) return nullptr;

    return _fTcpSrvGetUserByDescriptor(daemon, daemon->getConnections().at(idx));
}

const std::vector<int>& ftcp_server_daemon::getConnections() {
    return _socketInfo.acceptedConnections;
}

struct ftcp_server_user* _fTcpSrvGetUserByDescriptor(struct ftcp_server_daemon* daemon, int descriptor) {
    if (!daemon) return nullptr;

    auto user = daemon->getUser((long)descriptor);
    if (!user.has_value()) return nullptr;

    return std::addressof(user.value().get());
}

ftcp_server_daemon::~ftcp_server_daemon() {
    for (int desc : _socketInfo.acceptedConnections) {
        closeSocket(desc);
        finishSocket(desc);
    }
    closeSocket(_socketInfo.masterSocket);

    _stopThreads = true;
    
    if (FIGHTABLE_OUTPUT_ONLY_WARNINGS) printf("ftcp_server_daemon: waiting for worker threads to finish\n");

    _baseThreadValue.join();
    if (FIGHTABLE_OUTPUT_ONLY_WARNINGS) printf("ftcp_server_daemon: _baseThread is done\n");
    _eventThreadValue.join();
    if (FIGHTABLE_OUTPUT_ONLY_WARNINGS) printf("ftcp_server_daemon: _eventThread is done\n");
    _queueThreadValue.join();
    if (FIGHTABLE_OUTPUT_ONLY_WARNINGS) printf("ftcp_server_daemon: _queueThread is done\n");
}

void ftcp_server_daemon::finishSocket(int fd) {
    if (FIGHTABLE_OUTPUT_ONLY_WARNINGS) printf("ftcp_server_daemon: waiting for descriptor %d's thread to finish\n", fd);

    ftcp_server_daemon_user_context* ctx = _userThreads[fd];
    ctx->should_exit = true;
    ctx->thread.join();

    delete ctx;

    _userThreads.erase(fd);
}

typedef uint8_t fmd5_t[16];

std::string ftcp_server_daemon::getShortHash(const std::string &str) {
    fmd5_t md5;
    md5String((char *)str.c_str(), md5);

    std::string hash = GenericTools::valueToHex<fmd5_t>(md5);
    hash.erase(hash.length() - 20, 20);

    return hash;
}

bool ftcp_server_daemon::ready() {
    return _ready;
}
unsigned char _fTcpSrvReady(struct ftcp_server_daemon* daemon) {
    if (!daemon) return 0;

    return (unsigned char)daemon->ready();
}