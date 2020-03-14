#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define LOCAL_IMAGE_PROCESS

#include <thread>
#include <QMainWindow>
#include <QTime>

#include "port/port_handler.h"
#include "image_socket/image_socket.h"
#include "face_detector/face_detector.h"
#include "face_detector/identity_authencation.h"
#include "info_process/info_process.h"
#include "define.h"

#include "ui/add_face_window.h"
#include "ui/control_window.h"

#ifdef LOCAL_IMAGE_PROCESS
#include "image_process/image_process.h"
#endif // LOCAL_IMAGE_PROCESS

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

enum DataState {
    CLOSE = -1,
    PORT = 0,
    SERVER = 1,
};

enum SoundType {
    SPECIAL_PERSON_SOUND = 0,
    UNKNOWN_PERSON_SOUND = 1,
};

struct PASSING_DATA {
    PersonInfo info;
    QTime time;
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void update();
    void init();

signals:
    void reciveImage();

private slots:
    void on_SwitchButton_clicked();

    void on_CleanWindowButton_clicked();

    void on_UpdateButton_clicked();

    void on_ServerButton_clicked();

    void on_SendDataButton_clicked();

    void on_ConectComboBox_currentTextChanged(const QString &arg1);

    void on_turnLeftButton_clicked();

    void on_turnRightButton_clicked();

    void on_InputFace_clicked();

    void process();

    void on_ShowResultButton_clicked();

    void addFaceWindowClose();
    void controlWindowClose();

    void on_InfoManagerButton_clicked();

    void on_ResetTipsButton_clicked();

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
    bool show_detector_result_;

    int image_data_i_;  //记录图像数据位

    const int image_width_;
    const int image_height_;

    DataState current_state_;
    std::string current_ip_;

    AddFaceWindow *add_face_window_ = nullptr;
    ControlWindow *control_window_ = nullptr;
    cv::Mat recive_mat_;

    FaceDetector *face_detector_;
    IdentityAuthencation *identity_authencation_;

    InfoProcess *info_process_;
    void resetShowResultButton();

    std::string community_person_sound_;
    std::string special_person_sound_;
    std::string passing_info_file_;
    std::string special_case_file_;

    void playSound(SoundType type);
    void personManage(std::string id);
    void updatePassingInfo(PersonInfo info, std::string id);

    void addPassingInfo(Person person);
    void addSpecialCase(SpecialPerson special_person);
    PASSING_DATA last_passing_info_;
#ifdef LOCAL_IMAGE_PROCESS
    tf_image_process::ImageProcess *image_process_ ;
#endif // LOCAL_IMAGE_PROCESS

};
#endif // MAINWINDOW_H
