#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <thread>
#include <QMainWindow>

#include "port/port_handler.h"
#include "image_socket/image_socket.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

enum DataState {
    CLOSE = -1,
    PORT = 0,
    SERVER = 1,
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void update();
    void init();

private slots:
    void on_SwitchButton_clicked();

    void on_CleanWindowButton_clicked();

    void on_UpdateButton_clicked();

    void on_ServerButton_clicked();

    void on_SendDataButton_clicked();

    void on_ConectComboBox_currentTextChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    port_control::PortHandler *port_handler_;
    image_socket::ImageSocket *image_socket_;

    std::thread port_read_thread_;
    std::thread socket_read_thread_;
    bool switch_button_state_;

    void portReadThread();
    void socketReadThread();
    void addIP(const char* ip);
    void changeState(DataState state);
    bool ui_running_;

    int image_data_i_;  //记录图像数据位

    const int image_width_;
    const int image_height_;

    DataState current_state_;
    std::string current_ip_;
};
#endif // MAINWINDOW_H
