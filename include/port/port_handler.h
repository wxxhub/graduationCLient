#ifndef _PORT_HANDLER_H_
#define _PORT_HANDLER_H_

#include <vector>
#include <string>
#include "port_define.h"

namespace port_control {

class PortHandler {
public:
    static PortHandler *getPortHandler();
    bool isOpen(){return is_open_;}

    virtual ~PortHandler(){};

    virtual bool openPort(const char* port_name) = 0;
    virtual bool setBaudRate(const int baudrate) = 0;
    virtual bool setupPort() = 0;

    virtual void closePort() = 0;
    virtual void clearPort() = 0;
    virtual void setPortName(const char *port_name) = 0;

    virtual char *getPortName() = 0;

    virtual int getBaudRate() = 0;
    virtual int readPort(uint8_t *packet, int length) = 0;
    virtual int writePort(uint8_t *packet, int length) = 0;
    virtual int writePort(char *packet, int length) = 0;

    virtual std::vector<std::string> scanPort() = 0;

protected:
    bool is_open_;
};
}

#endif // _PORT_HANDLER_H_
