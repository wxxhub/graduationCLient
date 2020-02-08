#include "port/port_handler.h"
#include "port/port_handler_windows.h"

using namespace port_control;

PortHandler *PortHandler::getPortHandler() {
#if defined (_WIN320) || defined (_WIN64)
    return (PortHandler *)(new PortHandlerWindows());
#else
    #pragma message("Have't adpter yout system...")
#endif
}
