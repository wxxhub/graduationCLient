#ifndef _SOCKET_WINDOWS_H_
#define _SOCKET_WINDOWS_H_

#include <windows.h>
#include <thread>
#include <map>

#include "image_socket/image_socket.h"

namespace image_socket {

class SocketWindows : public ImageSocket{
public:
    SocketWindows();

    bool initSocket();
    bool updateSocket();

    void setPort(int port);
    void close();

    int readOneData(const std::string ip, char *data);

private:
    void acceptThread();

    SOCKET server_socket_;
    int port_;

    std::thread accept_thread_;
    std::map<std::string, SOCKET> clients_;
};

}

#endif // _SOCKET_WINDOWS_H_
