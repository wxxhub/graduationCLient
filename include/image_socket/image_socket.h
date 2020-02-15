#ifndef IMAGESOCKET_H
#define IMAGESOCKET_H

#include "define.h"
#include "image_socket/image_socket_define.h"

namespace image_socket {

class ImageSocket {
public:
    static ImageSocket *getSocketHandler();
    bool isOpen(){return is_open_;}
    void setAcceptCallback(CallbackFun fun){accept_callback_fun_ = fun;}

    virtual ~ImageSocket(){};

    virtual bool initSocket() = 0;
    virtual bool updateSocket() = 0;

    virtual void setPort(int port) = 0;
    virtual void closeServer() = 0;

    virtual int readOneData(const std::string ip, char *data) = 0;
protected:
    bool is_open_;
    CallbackFun accept_callback_fun_;
};

}

#endif // IMAGESOCKET_H
