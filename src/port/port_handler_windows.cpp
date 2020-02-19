#include <stdio.h>

#if defined (_WIN320) || defined (_WIN64)
#include "port/port_handler_windows.h"

using namespace port_control;

PortHandlerWindows::PortHandlerWindows()
    : baud_rate_(DEFAULT_BAUDREATE) {

    for (int i = 0; i < 10; i++) {
        char port_name[32];
        sprintf(port_name, "com%d", i);
        ports_.push_back(port_name);
    }
}

bool PortHandlerWindows::openPort(const char* port_name) {
    setPortName(port_name);
    return setBaudRate(baud_rate_);
}

bool PortHandlerWindows::setBaudRate(const int baudrate) {
    if (baudrate == baud_rate_)
        return true;

    baud_rate_ = baudrate;
}

bool PortHandlerWindows::setupPort() {
    DCB dcb;
    COMMTIMEOUTS timeouts;

    closePort();

    com_ = CreateFileA(port_name_, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (com_ == INVALID_HANDLE_VALUE) {
        printf("[PortHandlerWindows::SetupPort] Error opening serial port!\n");
        return false;
    }

    SetupComm(com_, INPUT_CACHE, OUTPUT_CACHE);

    GetCommState(com_, &dcb);
    dcb.BaudRate - baud_rate_; //波特率
    dcb.ByteSize = 8;  //每个字节有8位
    dcb.Parity = NOPARITY;  //无奇偶校验位
    dcb.StopBits = ONESTOPBIT;  //一个停止位
    SetCommState(com_, &dcb);

    COMMTIMEOUTS time_out;
    time_out.ReadIntervalTimeout = 0; //读间隔超时
    time_out.ReadTotalTimeoutMultiplier = 0; //读时间系数
    time_out.ReadTotalTimeoutConstant = 1; //读时间常量
    timeouts.WriteTotalTimeoutMultiplier = 0; //写时间系数
    timeouts.WriteTotalTimeoutConstant = 0; //写时间常量
    SetCommTimeouts(com_, &time_out);  //设置超时

    is_open_ = true;
    return true;
}

void PortHandlerWindows::closePort() {
    if (com_ != INVALID_HANDLE_VALUE) {
        CloseHandle(com_);
        com_ = INVALID_HANDLE_VALUE;
    }
    is_open_ = false;
}

void PortHandlerWindows::clearPort() {
    PurgeComm(com_, PURGE_RXABORT | PURGE_RXCLEAR);
}

void PortHandlerWindows::setPortName(const char *port_name) {
    strcpy_s(port_name_, sizeof(port_name_), port_name);
}

char *PortHandlerWindows::getPortName() {
    return port_name_;
}

int PortHandlerWindows::getBaudRate() {
    return baud_rate_;
}

int PortHandlerWindows::readPort(uint8_t *packet, int length){
    DWORD dwRead = 0;
    if (ReadFile(com_, packet, (DWORD)length, &dwRead, NULL) == FALSE)
        return -1;

    return  (int)dwRead;
}

int PortHandlerWindows::writePort(uint8_t *packet, int length) {
    DWORD dwWrite = 0;

  if (WriteFile(com_, packet, (DWORD)length, &dwWrite, NULL) == FALSE)
    return -1;

  return (int)dwWrite;
}

int PortHandlerWindows::writePort(char *packet, int length) {
    DWORD dwWrite = 0;

  if (WriteFile(com_, packet, (DWORD)length, &dwWrite, NULL) == FALSE)
    return -1;

  return (int)dwWrite;
}

void PortHandlerWindows::setPacketTimeout(double msec) {
    packet_start_time_ = getCurrentTime();
    packet_timeout_ = msec;
}

bool PortHandlerWindows::isPacketTimeout() {
    if (getTimeSinceStart() > packet_timeout_)
    {
        packet_timeout_ = 0;
        return true;
    }
    return false;
}

double PortHandlerWindows::getCurrentTime() {
    QueryPerformanceCounter(&counter_);
    QueryPerformanceFrequency(&freq_);
    return (double)counter_.QuadPart / (double)freq_.QuadPart * 1000.0;
}

double PortHandlerWindows::getTimeSinceStart() {
    double time;

    time = getCurrentTime() - packet_start_time_;
    if (time < 0.0) packet_start_time_ = getCurrentTime();

    return time;
}

std::vector<std::string> PortHandlerWindows::scanPort() {
    std::vector<std::string> openning_ports;

    std::vector<std::string>::iterator iter = ports_.begin();
    for (; iter != ports_.end(); iter++) {
        if (openPort(iter->c_str())) {
            printf("success open port %s\n", iter->c_str());
            std::string port_name = *iter;
            openning_ports.push_back(port_name);
        }
        closePort();
    }

    return openning_ports;
}

#endif // defined (_WIN320) || defined (_WIN64)
