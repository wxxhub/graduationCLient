#ifndef SOCKETLINUX_H
#define SOCKETLINUX_H

#if defined (linux)
#include <thread>
#include <map>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "image_socket/image_socket.h"

namespace image_socket {

class SocketLinux : public ImageSocket {
public:
    SocketLinux();
    ~SocketLinux(){closeServer();}

    bool initSocket();
    bool updateSocket();

    void setPort(int port);
    void closeServer();

    int readOneData(const std::string ip, char *data);

private:
    int server_socket_;
    int port_;
    struct sockaddr_in server_addr_;

    std::thread accept_thread_;
    std::map<std::string, int> clients_;

    void acceptThread();
};

}

#endif // defined (linux)
#endif // SOCKETLINUX_H
