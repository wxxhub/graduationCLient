#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <thread>
#include <QMainWindow>

#include "port/port_handler.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void update();
    void init();

private slots:
    void on_SwitchButton_clicked();

private:
    Ui::MainWindow *ui;
    port_control::PortHandler *port_handler_;

    std::thread port_read_thread_;
    bool switch_button_state_;

    void portReadThread();
    void updateSwitchButton();
    bool ui_running_;
};
#endif // MAINWINDOW_H
