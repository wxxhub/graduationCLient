#include "port/port_handler.h"

#if defined (_WIN320) || defined (_WIN64)
#include "port/port_handler_windows.h"
#elif defined(linux)
#include "port/port_handler_linux.h"
#else
    #pragma message("Have't adpter yout system...")
#endif

using namespace port_control;

PortHandler *PortHandler::getPortHandler() {
#if defined (_WIN320) || defined (_WIN64)
    return (PortHandler *)(new PortHandlerWindows());
#elif defined(linux)
    return (PortHandler *)(new PortHandlerLinux());
#else
    #pragma message("Have't adpter yout system...")
#endif
}
