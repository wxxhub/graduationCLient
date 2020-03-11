#include "include/ui/control_window.h"
#include "ui_control_window.h"

ControlWindow::ControlWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ControlWindow)
{
    ui->setupUi(this);
}

ControlWindow::~ControlWindow()
{
    delete ui;
}
