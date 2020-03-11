#ifndef CONTROL_WINDOW_H
#define CONTROL_WINDOW_H

#include <QMainWindow>

namespace Ui {
class ControlWindow;
}

class ControlWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ControlWindow(QWidget *parent = nullptr);
    ~ControlWindow();

private:
    Ui::ControlWindow *ui;

signals:
    void closeSignal();
};

#endif // CONTROL_WINDOW_H
