#ifndef ADD_FACE_WINDOW_H
#define ADD_FACE_WINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <thread>
#include <opencv2/opencv.hpp>

#include "face_detector/add_face.h"

namespace Ui {
class AddFaceWindow;
}

class AddFaceWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AddFaceWindow(QWidget *parent = nullptr);
    ~AddFaceWindow();

    bool running() {return running_;}
private slots:
    void on_SaveButton_clicked();

    void on_ChangeSavePath_clicked();

    void on_ResetButton_clicked();

private:
    Ui::AddFaceWindow *ui;
    std::thread capture_thread_;

    void init();
    void captureThread();

    const int image_width_;
    const int image_height_;

    bool running_;

    AddFace *add_face_;

    QStandardItemModel *preview_model_;
    int set_item_index_ = 0;

signals:
    void closeSignal();
};

#endif // ADD_FACE_WINDOW_H
