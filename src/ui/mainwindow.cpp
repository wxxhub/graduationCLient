#include <iostream>
#include <time.h>

#include <QMessageBox>

#include "ui/mainwindow.h"
#include "ui_mainwindow.h"

#if defined (_WIN32) || defined (_WIN64)
#include <Windows.h>
#else
    #pragma message("Have't adpter yout system...")
#endif

using namespace std;

static void sleep_ms(int time) {
#if defined (_WIN32) || defined (_WIN64)
    Sleep(time);
#else
    #pragma message("Have't adpter yout system...")
#endif
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui_running_ = true;
    switch_button_state_ = false;

    port_handler_ = port_control::PortHandler::getPortHandler();
    port_read_thread_ = std::thread(&MainWindow::portReadThread, this);

    ui->setupUi(this);

    init();
    update();
}

MainWindow::~MainWindow() {
    ui_running_ = false;
    port_read_thread_.join();
    delete ui;    
}

void MainWindow::update() {
    vector<string> ports = port_handler_->scanPort();

    ui->changePortComboBox->clear();
    vector<string>::iterator iter = ports.begin();
    for (; iter != ports.end(); iter++) {
        ui->changePortComboBox->addItem(QString(iter->c_str()));
    }
}

void MainWindow::init() {
    // init button
    updateSwitchButton();

    // init baudrate list
    std::vector<int> baudrate_list = {
        9600,
        19200,
        38400,
        57600,
        115200,
        230400,
        460800,
        500000,
        576000,
        921600,
        1000000,
        1152000,
        1500000,
        2000000,
    };

   vector<int>::iterator iter = baudrate_list.begin();
   for (; iter != baudrate_list.end(); iter++) {
        ui->baudrateComboBox->addItem(QString::number(*iter));
   }
}

void MainWindow::portReadThread() {
    while (ui_running_) {
//        cout << "portReadThread" << endl;
        if (port_handler_->isOpen()) {
            uint8_t data;
            if(port_handler_->readPort(&data, 1)) {
                char code[8];
                sprintf(code, "%x", int(data));
//                ui->readDataText->append(QString(code));
                ui->readDataText->setText("test");
            }

        } else {
//            cout << "port not open" << endl;
        }

        sleep_ms(1);
    }
}

void MainWindow::updateSwitchButton(){
    if (switch_button_state_) {
        ui->SwitchButton->setText("关闭");
    } else {
        ui->SwitchButton->setText("开启");
    }
}

void MainWindow::on_SwitchButton_clicked()
{
    switch_button_state_ = !switch_button_state_;

    if (switch_button_state_) {
        const char *port_name = ui->changePortComboBox->currentText().toStdString().c_str();
        int baudrate = ui->baudrateComboBox->currentText().toInt();
        port_handler_->setBaudRate(baudrate);
        if(port_handler_->openPort(port_name)) {
            updateSwitchButton();
        } else {
            QMessageBox::warning(this, "open port failed", "open port failed!");
            switch_button_state_ = !switch_button_state_;
        }
        cout << "open port" << endl;
    } else {
        port_handler_->closePort();
        updateSwitchButton();
        cout << "close port" << endl;
    }
}
