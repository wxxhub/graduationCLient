#ifndef PORT_HANDLER_LINUX_H
#define PORT_HANDLER_LINUX_H

#if defined (linux)
#include "port/port_handler.h"
namespace port_control {

class PortHandlerLinux : public PortHandler {
public:
    PortHandlerLinux();
    virtual ~PortHandlerLinux() {closePort();}

    bool openPort(const char* port_name);
    bool setBaudRate(const int baudrate);
    bool setupPort();

    void closePort();
    void clearPort();
    void setPortName(const char *port_name);

    char *getPortName();

    int getBaudRate();
    int readPort(uint8_t *packet, int length);
    int writePort(uint8_t *packet, int length);

    std::vector<std::string> scanPort();

private:
    int handler_fd_;
    int baud_rate_;
    char port_name_[32];

    double packet_start_time_;
    double packet_timeout_;
    double tx_time_per_byte_;

    int getCFlagBaud(int baud_rate);
};
}

#endif // defined (linux)
#endif // PORT_HANDLER_LINUX_H
