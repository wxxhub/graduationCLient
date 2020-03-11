#include <iostream>

#include "ui/control_window.h"
#include "ui_control_window.h"

ControlWindow::ControlWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ControlWindow) {

    setAttribute(Qt::WA_DeleteOnClose);

    ui->setupUi(this);
}

ControlWindow::~ControlWindow() {
    std::cout << "~ControlWindow()" << std::endl;
    emit closeSignal();
    delete ui;
}
