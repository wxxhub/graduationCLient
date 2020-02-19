#if defined (_WIN320) || defined (_WIN64)
#include <iostream>
#include "image_socket/socket_windows.h"

using namespace std;
using namespace image_socket;

SocketWindows::SocketWindows()
    : port_(DEFAULT_PORT){

}


bool SocketWindows::initSocket() {
    WORD sock_version = MAKEWORD(2, 2);
    WSADATA wsa_data;

    if (WSAStartup(sock_version, &wsa_data) != 0) {

        return false;
    }

     return updateSocket();
}

bool SocketWindows::updateSocket() {
    //创建套接字
    server_socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

     if (server_socket_ == INVALID_SOCKET) {
         cout << "server_socket_ error!" << endl;
         return false;
     }

    sockaddr_in sin;

    sin.sin_family = AF_INET;
    sin.sin_port = htons(port_);
    sin.sin_addr.S_un.S_addr = INADDR_ANY;

    // 绑定IP和端口
    if (bind(server_socket_, (LPSOCKADDR)&sin, sizeof (sin)) == SOCKET_ERROR) {
        cout << "bind error !" << endl;
        return false;
    }

    // 开始监听
    if (listen(server_socket_, 5) == SOCKET_ERROR) {
        cout << "listen error !" << endl;
        return false;
    }
    is_open_ = true;
    accept_thread_ = std::thread(&SocketWindows::acceptThread, this);
     //绑定IP和端口
    return true;
}

void SocketWindows::setPort(int port) {
    port_ = port;
}

void SocketWindows::closeServer() {
    is_open_ = false;
    closesocket(server_socket_);
    accept_thread_.join();
}

void SocketWindows::acceptThread() {
    is_open_ = true;
    sockaddr_in remote_addr;
    int addr_len = sizeof(remote_addr);

    while (is_open_) {
        SOCKET client = accept(server_socket_, (SOCKADDR *)&remote_addr, &addr_len);
        if (client == INVALID_SOCKET) {
            cout << "accept error!" << endl;
            continue;
        }

        string ip = inet_ntoa(remote_addr.sin_addr);
        clients_[ip] = client;
        accept_callback_fun_(ip.c_str());
    }
}

int SocketWindows::readOneData(const std::string &ip, char *data) {
    return recv(clients_[ip], data, 1, 0);
}

int SocketWindows::writeData(const std::string &ip, char *data, int len) {

}

#endif // defined (_WIN320) || defined (_WIN64)
