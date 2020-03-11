#include "ui/add_face_window.h"
#include "ui_add_face_window.h"

#include <QDateEdit>
#include <QFileDialog>
#include <QMessageBox>
#include <QString>
#include <QTableWidget>

#include <mutex>

using namespace std;
using namespace cv;

mutex running_mutex_;

AddFaceWindow::AddFaceWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AddFaceWindow)
    , image_width_(640)
    , image_height_(480) {

    setAttribute(Qt::WA_DeleteOnClose);

    ui->setupUi(this);
    init();
}

AddFaceWindow::~AddFaceWindow() {
    running_mutex_.lock();
    running_ = false;
    running_mutex_.unlock();

    capture_thread_.join();

    preview_model_->clear();
    emit closeSignal();
    delete ui;
}

void AddFaceWindow::init() {
    running_ = true;

    add_face_ = new AddFace();
    add_face_->init("/home/wxx/Project/c++/face_detector/models/mtcnn_frozen_model.pb");
    preview_model_ = new QStandardItemModel();
    ui->PreviewTableView->setModel(preview_model_);
    capture_thread_ = std::thread(&AddFaceWindow::captureThread, this);
}

void AddFaceWindow::captureThread() {
    VideoCapture capture(0);

    capture.set(CAP_PROP_FRAME_WIDTH, image_width_);
    capture.set(CAP_PROP_FRAME_HEIGHT, image_height_);

    if (!capture.isOpened()) {
        cerr<<"failed to open camera"<<endl;
        return;
    }

    Mat frame;

    while (running_) {
        running_mutex_.unlock();
        capture >> frame;

        if (add_face_->process(frame)) {

            bool finish;
            Mat face_mat = add_face_->getCurrentImage(finish);

            QLabel *new_label = new QLabel(" ");
            QImage face_image(face_mat.data, face_mat.cols, face_mat.rows, QImage::Format_Grayscale8);
            QPixmap face_pix_map = QPixmap::fromImage(face_image);
            new_label->setPixmap(face_pix_map);
            QIcon icon;
            icon.addPixmap(face_pix_map);
            preview_model_->setItem(0, set_item_index_++, new QStandardItem(icon, ""));

            if (finish) {
                QMessageBox::warning(this, "提示", "采集完成,你可以保存数据或着重新采集!");
            }
        }

        QImage image(frame.data, frame.cols, frame.rows, QImage::Format_BGR888);
        QPixmap pix_map = QPixmap::fromImage(image);
        ui->CaptureView->setPixmap(pix_map);

        running_mutex_.lock();
    }

    capture.release();
}

void AddFaceWindow::on_SaveButton_clicked() {
    string name = ui->NameLineEdit->text().toStdString();
    string path = ui->SavePathLineEdit->text().toStdString();
    if (path[path.size() - 1] != '/') {
        path += '/';
    }

    string full_path = path+name;

    QDir save_dir(full_path.c_str());

    if (save_dir.exists()) {
        QMessageBox::warning(this, "错误", "成员已存在!请修改名称!");
        return;
    }

    if (!save_dir.mkdir(full_path.c_str())) {
        QMessageBox::warning(this, "错误", "创建目录失败!");
        return;
    }

    add_face_->setSave(path, name);
    SAVE_STATUS status = add_face_->save();


    if (status == SAVE_SUCCESS) {
        add_face_->reset();
        QMessageBox::warning(this, "成功", "保存成功!");
    } else if (status == EXIST) {
        QMessageBox::warning(this, "错误", "成员已存在!请修改名称!");
    } else {
        QMessageBox::warning(this, "失败", "保存失败!");
    }
}

void AddFaceWindow::on_ChangeSavePath_clicked() {
    QString dirpath = QFileDialog::getExistingDirectory(this,"选择目录","./",QFileDialog::ShowDirsOnly);
    ui->SavePathLineEdit->setText(dirpath);
}

void AddFaceWindow::on_ResetButton_clicked() {
    add_face_->reset();

    preview_model_->clear();
    set_item_index_ = 0;
}
