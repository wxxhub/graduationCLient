QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#INCLUDEPATH += \
#        ./include

INCLUDEPATH = "./include/"

SOURCES += \
    src/face_detector/add_face.cpp \
    src/face_detector/face_detector.cpp \
    src/face_detector/face_mat_process.cpp \
    src/image_process/image_process.cpp \
    src/image_socket/image_socket.cpp \
    src/image_socket/socket_linux.cpp \
    src/image_socket/socket_windows.cpp \
    src/main.cpp \
    src/mtnn/comm_lib.cpp \
    src/mtnn/tensorflow_mtcnn.cpp \
    src/mtnn/utils.cpp \
    src/port/port_handler.cpp \
    src/port/port_handler_linux.cpp \
    src/port/port_handler_windows.cpp \
    src/ui/add_face_window.cpp \
    src/ui/mainwindow.cpp

HEADERS += \
    include/define.h \
    include/face_detector/add_face.h \
    include/face_detector/face_detector.h \
    include/face_detector/face_mat_process.h \
    include/image_process/image_process.h \
    include/image_socket/image_socket.h \
    include/image_socket/image_socket_define.h \
    include/image_socket/socket_linux.h \
    include/image_socket/socket_windows.h \
    include/mtnn/comm_lib.hpp \
    include/mtnn/mtcnn.hpp \
    include/mtnn/tensorflow_mtcnn.hpp \
    include/mtnn/utils.hpp \
    include/port/port_define.h \
    include/port/port_handler_linux.h \
    include/port/port_handler_windows.h \
    include/ui/add_face_window.h \
    include/ui/mainwindow.h \
    include/port/port_handler.h \
    include/timer.h

FORMS += \
    ui/add_face_window.ui \
    ui/mainwindow.ui

TRANSLATIONS += \
    Control_zh_CN.ts

win32:{
    LIBS += -lpthread libwsock32 libws2_32
}
win64:{
    LIBS += -lpthread libwsock32 libws2_32
}
linux:{
INCLUDEPATH += /home/wxx/lib/OpenCV4.2.0-gpu/include/opencv4 \
               /home/wxx/lib/OpenCV4.2.0-gpu/include/opencv4/opencv2 \
               /home/wxx/lib/tensorflow/include \

LIBS += /home/wxx/lib/OpenCV4.2.0-gpu/lib/libopencv_highgui.so \
        /home/wxx/lib/OpenCV4.2.0-gpu/lib/libopencv_core.so \
        /home/wxx/lib/OpenCV4.2.0-gpu/lib/libopencv_imgcodecs.so \
        /home/wxx/lib/OpenCV4.2.0-gpu/lib/libopencv_objdetect.so \
        /home/wxx/lib/OpenCV4.2.0-gpu/lib/libopencv_imgproc.so \
        /home/wxx/lib/OpenCV4.2.0-gpu/lib/libopencv_video.so \
        /home/wxx/lib/OpenCV4.2.0-gpu/lib/libopencv_videoio.so \
        /home/wxx/lib/OpenCV4.2.0-gpu/lib/libopencv_dnn.so \
        /home/wxx/lib/tensorflow/lib/libtensorflow.so \
        /home/wxx/lib/tensorflow/lib/libtensorflow_framework.so \
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    README.md
