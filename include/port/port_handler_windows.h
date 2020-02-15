#ifndef _PORT_HANDLER_WINDOWS_H_
#define _PORT_HANDLER_WINDOWS_H_

#if defined (_WIN320) || defined (_WIN64)
#include <windows.h>

#include "port/port_handler.h"

namespace port_control {

class PortHandlerWindows : public PortHandler {
public:
    PortHandlerWindows();
    virtual ~PortHandlerWindows() {closePort();}

    bool openPort(const char* port_name);
    bool setBaudRate(const int baudrate);
    bool setupPort(const int baud_rate);

    void closePort();
    void clearPort();
    void setPortName(const char *port_name);

    char *getPortName();

    int getBaudRate();
    int readPort(uint8_t *packet, int length);
    int writePort(uint8_t *packet, int length);

    std::vector<std::string> scanPort();

private:
    int baud_rate_;
    char port_name_[32];

    double packet_start_time_;
    double packet_timeout_;
    double tx_time_per_byte_;

    std::vector<std::string> ports_;
};

}

#endif // defined (_WIN320) || defined (_WIN64)
#endif // _PORT_HANDLER_WINDOWS_H_
