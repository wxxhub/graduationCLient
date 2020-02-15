#if defined (linux)
#include "image_socket/socket_linux.h"
#include "image_socket/image_socket_define.h"

using namespace std;
using namespace image_socket;

SocketLinux::SocketLinux()
    : port_(DEFAULT_PORT) {

}

bool SocketLinux::initSocket() {
    is_open_ = false;
    return updateSocket();
}

bool SocketLinux::updateSocket() {
    closeServer();

    server_socket_ = socket(AF_INET, SOCK_STREAM, 0);

    if (server_socket_ < 0) {
        printf("create socket error!\n");
        return false;
    }

    server_addr_.sin_family = AF_INET;
    server_addr_.sin_port = htons(port_);
    server_addr_.sin_addr.s_addr = INADDR_ANY;

    int bind_fd = bind(server_socket_, (struct sockaddr*)&server_addr_, sizeof (server_addr_));
    if (bind_fd < 0) {
        printf("bind error!\n");
        return false;
    }

    int listen_fd = listen(server_socket_, 5);

    if (listen_fd < 0) {
        printf("listen error!\n");
        return false;
    }
    is_open_ = true;
    accept_thread_ = std::thread(&SocketLinux::acceptThread, this);
    return true;
}

void SocketLinux::setPort(int port) {
    port_ = port;
}

void SocketLinux::closeServer() {
    if (is_open_) {
        is_open_ = false;

        close(server_socket_);

        accept_thread_.join();
    }
}

int SocketLinux::readOneData(const std::string ip, char *data) {
    return recv(clients_[ip], data, 1, 0);
}

void SocketLinux::acceptThread() {
    is_open_ = true;
    struct sockaddr_in remote_addr;
    socklen_t addr_len = sizeof(remote_addr);

    while (is_open_) {
        int client = accept(server_socket_, (struct sockaddr*)&remote_addr, &addr_len);
        if (client < 0) {
            printf("accept error!\n");
            continue;
        }

        string ip = inet_ntoa(remote_addr.sin_addr);
        clients_[ip] = client;
        accept_callback_fun_(ip.c_str());
    }
}

#endif // defined (linux)
