#if defined (linux)
#include <unistd.h>
#include <fcntl.h>
#include <linux/serial.h>
#include <termios.h>
#include <iostream>
#include <string.h>
#include <dirent.h>

#include "port/port_handler_linux.h"
#include "port/port_define.h"

using namespace std;
using namespace port_control;

char port_name_type[6] = {'t', 't', 'y', 'U', 'S', 'B'};

PortHandlerLinux::PortHandlerLinux()
    : baud_rate_(DEFAULT_BAUDREATE) {
}

bool PortHandlerLinux::openPort(const char* port_name) {
    setPortName(port_name);

    return setupPort();
}

bool PortHandlerLinux::setBaudRate(const int baudrate) {
    if (baudrate == baud_rate_)
        return true;

    baud_rate_ = baudrate;
    return true;
}

bool PortHandlerLinux::setupPort() {
    closePort();

    struct termios newtio;

    handler_fd_ = open(port_name_, O_RDWR|O_NOCTTY|O_NONBLOCK);
    if(handler_fd_ < 0)
    {
        printf("[PortHandlerLinux::SetupPort] Error opening serial port!\n");
        return false;
    }

    bzero(&newtio, sizeof(newtio)); // clear struct for new port settings

    newtio.c_cflag = getCFlagBaud(baud_rate_) | CS8 | CLOCAL | CREAD;
    newtio.c_iflag = IGNPAR;
    newtio.c_oflag      = 0;
    newtio.c_lflag      = 0;
    newtio.c_cc[VTIME]  = 0;
    newtio.c_cc[VMIN]   = 0;

    // clean the buffer and activate the settings for the port
    tcflush(handler_fd_, TCIFLUSH);
    tcsetattr(handler_fd_, TCSANOW, &newtio);

    return true;
}

void PortHandlerLinux::closePort() {
    if (handler_fd_ != -1)
        close(handler_fd_);

    handler_fd_ = -1;
}

void PortHandlerLinux::clearPort() {
     tcflush(handler_fd_, TCIFLUSH);
}

void PortHandlerLinux::setPortName(const char *port_name) {
    strcpy(port_name_, port_name);
}

char *PortHandlerLinux::getPortName() {
    return port_name_;
}

int PortHandlerLinux::getBaudRate() {
    return baud_rate_;
}

int PortHandlerLinux::readPort(uint8_t *packet, int length) {
    return read(handler_fd_, packet, length);
}

int PortHandlerLinux::writePort(uint8_t *packet, int length) {
    return write(handler_fd_, packet, length);
}

int PortHandlerLinux::writePort(char *packet, int length) {
    return write(handler_fd_, packet, length);
}


vector<string> PortHandlerLinux::scanPort() {
    cout << "try open /dev/" << endl;
    vector<string> ports;

    DIR *opening_dir = opendir( "/dev/");

    if (opening_dir == NULL) {
        cout << "open /dev/ failed!" << endl;
        return ports;
    }

    dirent *p = nullptr;

    while ((p = readdir(opening_dir)) != nullptr) {
        int i = 0;
        for (; i < 6; i++) {
            if (port_name_type[i] != p->d_name[i])
                break;
        }

        if (i == 6) {
//            cout << p->d_name << endl;
            ports.push_back(p->d_name);
        }
    }
    return ports;
}

int PortHandlerLinux::getCFlagBaud(int baud_rate) {
    switch (baud_rate) {
        case 9600:
            return B9600;
        case 19200:
            return B19200;
        case 38400:
            return B38400;
        case 57600:
            return B57600;
        case 115200:
            return B115200;
        case 230400:
            return B230400;
        case 460800:
            return B460800;
        case 500000:
            return B500000;
        case 576000:
            return B576000;
        case 921600:
            return B921600;
        case 1000000:
            return B1000000;
        case 1152000:
            return B1152000;
        case 1500000:
            return B1500000;
        case 2000000:
            return B2000000;
        case 2500000:
            return B2500000;
        case 3000000:
            return B3000000;
        case 3500000:
            return B3500000;
        case 4000000:
            return B4000000;
        default:
            return -1;
    }
}

#endif // defined (linux)
