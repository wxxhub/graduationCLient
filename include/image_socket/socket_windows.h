#ifndef _SOCKET_WINDOWS_H_
#define _SOCKET_WINDOWS_H_

#if defined (_WIN320) || defined (_WIN64)
#include <windows.h>
#include <thread>
#include <map>

#include "image_socket/image_socket.h"

namespace image_socket {

class SocketWindows : public ImageSocket{
public:
    SocketWindows();
    ~SocketWindows(){closeServer();}

    bool initSocket();
    bool updateSocket();

    void setPort(int port);
    void closeServer();

    int readOneData(const std::string ip, char *data);

private:
    void acceptThread();

    int server_socket_;
    int port_;

    std::thread accept_thread_;
    std::map<std::string, SOCKET> clients_;
};

}
#endif // defined (_WIN320) || defined (_WIN64)
#endif // _SOCKET_WINDOWS_H_
