#include <iostream>
#include <time.h>

#include <QMessageBox>

#include "ui/mainwindow.h"
#include "ui_mainwindow.h"
#include "timer.h"

using namespace std;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , image_width_(320)
    , image_height_(240)
    , current_state_(CLOSE){

    ui_running_ = true;
    switch_button_state_ = false;

    port_handler_ = port_control::PortHandler::getPortHandler();
    image_socket_ = image_socket::ImageSocket::getSocketHandler();
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
        cout << *iter << endl;
        ui->changePortComboBox->addItem(QString(iter->c_str()));
    }
}

void MainWindow::init() {
    // init button
   // updateSwitchButton();
    current_ip_ = "192.168.31.129";

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

   // init image
   QImage image("C:\\Users\\wxx\\Pictures\\Screenshots\\屏幕截图(1).png");
   QPixmap pix_map = QPixmap::fromImage(image);
   ui->ImageView->setPixmap(pix_map);

   // image init
   ui->ImageProgressBar->setRange(0, 100);
   ui->ImageProgressBar->setValue(0);
}

void MainWindow::portReadThread() {
    image_data_i_ = 0;
    const int size = image_width_ * image_height_;
    uchar image_data[size];

    string cache = "";
    while (ui_running_) {
//        cout << "portReadThread" << endl;
        if (port_handler_->isOpen() && switch_button_state_) {
            uint8_t data;
            if(port_handler_->readPort(&data, 1)) {
                if (data == 0XFF) { // update image
                    image_data_i_ = 0;
                    QImage image(image_data, image_width_, image_height_, QImage::Format_Grayscale8);
                    QPixmap pix_map = QPixmap::fromImage(image);
                    ui->ImageView->setPixmap(pix_map);
                    ui->ImageProgressBar->setValue(0);
                }
                image_data[image_data_i_] = data;

                if (image_data_i_ % 100 == 0) {
                    int value = image_data_i_ * 100 / size;
                    if (value < 100) {
//                        ui->ImageProgressBar->setValue(value);
                    }

                    ui->readDataText->insertPlainText(QString(cache.c_str()));
                    ui->readDataText->moveCursor(QTextCursor::End);

                    cache = "";
                }
                image_data_i_++;
                char code[4] = {'0', '0'};
                sprintf(code, "%2x\n", int(data));
                cache += code;
            }

        } else {
//            cout << "port not open" << endl;
        }

//        sleep_ms(1);
    }
}

void MainWindow::socketReadThread() {
    image_data_i_ = 0;
    const int size = image_width_ * image_height_;
    uchar image_data[size];

    string cache = "";
    ClockTime clock_time;
    clock_time.reset();
    cout << "socketReadThread" << endl;
    while (ui_running_ && current_state_ == SERVER) {
        if (image_socket_->isOpen()) {
            char data;
//            cout << "image_socket_->isOpen()" << endl;
            if(image_socket_->readOneData("192.168.31.204", &data) > 0) {
                uint8_t u_data = 0XFF & data;
//                cout << int(u_data) << endl;
                if (u_data == 0XFF) { // update image
                    image_data_i_ = 0;
                    QImage image(image_data, image_width_, image_height_, QImage::Format_Grayscale8);
                    QPixmap pix_map = QPixmap::fromImage(image);
                    ui->ImageView->setPixmap(pix_map);
                    cout << clock_time.clockMs() << "ms" << endl;
                }
                image_data[image_data_i_] = u_data;

//                if (image_data_i_ % 100 == 0) {
//                    int value = image_data_i_ * 100 / size;
//                    if (value < 100) {
//                        ui->ImageProgressBar->setValue(value);
//                    }

//                    ui->readDataText->insertPlainText(QString(cache.c_str()));
//                    ui->readDataText->moveCursor(QTextCursor::End);

//                    cache = "";
//                }
                image_data_i_++;
//                char code[4] = {'0', '0'};
//                sprintf(code, "%2x\n", int(*data));
//                cache += code;
            }
        }
    }
     cout << "socketReadThread finished!" << endl;
}

void MainWindow::addIP(const char* ip) {
    ui->ConectComboBox->addItem(QString(ip));
    current_ip_ = ui->ConectComboBox->currentText().toStdString();
}

void changeState(DataState state) {
    switch (state) {
    case PORT: {
        break;
    }
    case SERVER: {
        break;
    }
    case CLOSE:{
        break;
    }
    }
}

void MainWindow::on_SwitchButton_clicked()
{
//    switch_button_state_ = !switch_button_state_;

//    if (switch_button_state_) {
//        const char *port_name = ui->changePortComboBox->currentText().toStdString().c_str();
//        int baudrate = ui->baudrateComboBox->currentText().toInt();
//        port_handler_->setBaudRate(baudrate);
//        if(port_handler_->openPort(port_name)) {
//            updateSwitchButton();
//        } else {
//            QMessageBox::warning(this, "open port failed", "open port failed!");
//            switch_button_state_ = !switch_button_state_;
//        }
//        cout << "open port" << endl;
//    } else {
//        port_handler_->closePort();
//        updateSwitchButton();
//        cout << "close port" << endl;
//    }
}

void MainWindow::on_CleanWindowButton_clicked()
{
    ui->readDataText->clear();
}

void MainWindow::on_UpdateButton_clicked()
{
    update();
}

void MainWindow::on_ServerButton_clicked()
{
    if (current_state_ != SERVER) {
        current_state_ = SERVER;
        if (image_socket_->initSocket()) {
            CallbackFun fun = bind(&MainWindow::addIP, this, placeholders::_1);
            image_socket_->setAcceptCallback(fun);
            socket_read_thread_ = std::thread(&MainWindow::socketReadThread, this);
        } else {
            QMessageBox::warning(this, "open socket failed", "open socket failed!");
        }
    }
}
