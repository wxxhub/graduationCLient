#include "image_socket/image_socket.h"

#if defined (_WIN320) || defined (_WIN64)
#include "image_socket/socket_windows.h"
#elif defined (linux)
#include "image_socket/socket_linux.h"
#else
    #pragma message("Have't adpter yout system...")
#endif

using namespace image_socket;

ImageSocket *ImageSocket::getSocketHandler() {
#if defined (_WIN320) || defined (_WIN64)
    return (ImageSocket *)(new SocketWindows());
#elif defined (linux)
    return (ImageSocket *)(new SocketLinux());
#else
    #pragma message("Have't adpter yout system...")
#endif
}
