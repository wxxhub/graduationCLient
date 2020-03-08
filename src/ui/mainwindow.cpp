#include <iostream>
#include <time.h>

#include <QMessageBox>

#include "ui/mainwindow.h"
#include "ui_mainwindow.h"
#include "timer.h"

using namespace std;

const int MAX_TURN_DISTANCE = 50;

char turn_left_data[MAX_TURN_DISTANCE];
char turn_right_data[MAX_TURN_DISTANCE];

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
#ifdef LOCAL_IMAGE_PROCESS
    delete image_process_;
#endif // LOCAL_IMAGE_PROCESS
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
#ifdef LOCAL_IMAGE_PROCESS
    image_process_ = new tf_image_process::ImageProcess("/home/wxx/Project/TensorflowTest/c_test/ssd_inception_v2_coco_2017_11_17/frozen_inference_graph.pb",
                                                        "/home/wxx/Project/TensorflowTest/c_test/ssd_inception_v2_coco_2017_11_17/test.pbtxt");
#endif // LOCAL_IMAGE_PROCESS
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

   // distance spinBox init
   ui->distanceSpinBox->setPrefix("旋转步长");
   ui->distanceSpinBox->setMinimum(1);
   ui->distanceSpinBox->setMaximum(MAX_TURN_DISTANCE);
   for (int i = 0; i < MAX_TURN_DISTANCE; i++) {
       turn_left_data[i] = 'l';
       turn_right_data[i] = 'r';
   }
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
//           cout << "[" << current_ip_ << "]" << endl;
            char data;
//            cout << "image_socket_->isOpen()" << endl;
            if(image_socket_->readOneData(current_ip_, &data) > 0) {
                uint8_t u_data = 0XFF & data;
//                cout << int(u_data) << endl;
                if (u_data == 0XFF) { // update image
                    image_data_i_ = 0;
                    cout << clock_time.clockMs() << "ms" << endl;
#ifdef LOCAL_IMAGE_PROCESS
                    cv::Mat cv_image = cv::Mat(image_height_,image_width_, CV_8UC1, image_data);
                    cv::Mat rgb_image;
                    cvtColor(cv_image,rgb_image,CV_GRAY2BGR);

#if 0
                    cv::Mat result_image = image_process_->process(rgb_image);
                    QImage image(result_image.data, image_width_, image_height_, QImage::Format_BGR888);
#else
                    QImage image(rgb_image.data, image_width_, image_height_, QImage::Format_BGR888);
#endif // process image

                    QPixmap pix_map = QPixmap::fromImage(image);
                    ui->ImageView->setPixmap(pix_map);
//                    cv::imshow("image", cv_image);
//                    cout << "cv" << endl;
//                    cv::waitKey(10);
//                    image_process_
#endif // LOCAL_IMAGE_PROCESS
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
    ui->ConectComboBox->update();
//    ui->ConectComboBox->setCurrentText();
//    current_ip_ = ui->ConectComboBox->currentData().toString().toStdString();
//    current_ip_ = ui->ConectComboBox->currentText().toStdString();
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

void MainWindow::on_SendDataButton_clicked()
{
    string data = ui->SendDataLine->text().toStdString();
    switch (current_state_) {
    case PORT: {
        if (port_handler_->isOpen()) {
            port_handler_->writePort(const_cast<char *>(data.c_str()), data.length());
        }
        break;
    }
    case SERVER: {
        if (image_socket_->isOpen()) {
            image_socket_->writeData(current_ip_, const_cast<char *>(data.c_str()), data.length());
        }
        break;
    }
    case CLOSE:{
        QMessageBox::warning(this, "提示", "没有打开通讯接口!");
        break;
    }
    }
}

void MainWindow::on_ConectComboBox_currentTextChanged(const QString &arg1) {
    current_ip_ = arg1.toStdString();
}


void MainWindow::on_turnLeftButton_clicked() {
    int distance = ui->distanceSpinBox->value();
    distance = distance <= MAX_TURN_DISTANCE ? distance : MAX_TURN_DISTANCE;

    image_socket_->writeData(current_ip_, turn_left_data, distance);
}

void MainWindow::on_turnRightButton_clicked() {
    int distance = ui->distanceSpinBox->value();
    distance = distance <= MAX_TURN_DISTANCE ? distance : MAX_TURN_DISTANCE;

    image_socket_->writeData(current_ip_, turn_right_data, distance);
}

void MainWindow::on_InputFace_clicked() {
    if (add_face_window == nullptr) {
        add_face_window = new AddFaceWindow();
        add_face_window->show();
    }
}
