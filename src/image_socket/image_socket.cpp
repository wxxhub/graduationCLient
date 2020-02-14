#include "image_socket\image_socket.h"

#include "image_socket\socket_windows.h"

using namespace image_socket;

ImageSocket *ImageSocket::getSocketHandler() {
#if defined (_WIN320) || defined (_WIN64)
    return (ImageSocket *)(new SocketWindows());
#else
    #pragma message("Have't adpter yout system...")
#endif
}
