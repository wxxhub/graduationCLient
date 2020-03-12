#include <QDir>
#include <QMessageBox>
#include <iostream>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonParseError>

#include "ui/control_window.h"
#include "ui_control_window.h"

using namespace std;

ControlWindow::ControlWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ControlWindow) {

    setAttribute(Qt::WA_DeleteOnClose);

    // init path
    string applition_path =  QCoreApplication::applicationDirPath().toStdString();
    data_path_ = applition_path + "/data/";
    community_personnel_file_ = data_path_ + "community_personnel.json";
    special_peronnel_file_ = data_path_ + "special_peronnel.json";
    passing_info_file_ = data_path_ + "passing_info.json";
    exception_case_file_ = data_path_ + "exception_case.json";

    checkFile();

    // init model
    community_personnel_model_ = new QStandardItemModel();
    community_personnel_model_->setColumnCount(4);
    community_personnel_model_->setHeaderData(0, Qt::Horizontal,QString::fromLocal8Bit("选择"));
    community_personnel_model_->setHeaderData(1, Qt::Horizontal,QString::fromLocal8Bit("ID"));
    community_personnel_model_->setHeaderData(2, Qt::Horizontal,QString::fromLocal8Bit("姓名"));
    community_personnel_model_->setHeaderData(3, Qt::Horizontal,QString::fromLocal8Bit("照片"));

    special_peronnel_model_ = new QStandardItemModel();
    special_peronnel_model_->setColumnCount(5);
    special_peronnel_model_->setHeaderData(0, Qt::Horizontal,QString::fromLocal8Bit("选择"));
    special_peronnel_model_->setHeaderData(1, Qt::Horizontal,QString::fromLocal8Bit("ID"));
    special_peronnel_model_->setHeaderData(2, Qt::Horizontal,QString::fromLocal8Bit("姓名"));
    special_peronnel_model_->setHeaderData(3, Qt::Horizontal,QString::fromLocal8Bit("备注"));
    special_peronnel_model_->setHeaderData(4, Qt::Horizontal,QString::fromLocal8Bit("照片"));

    updateTable();
    ui->setupUi(this);

    // set table
    ui->CommunityPersonnelTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->CommunityPersonnelTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->SpecialPersonnelTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->SpecialPersonnelTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // set model
    ui->CommunityPersonnelTable->setModel(community_personnel_model_);
    ui->SpecialPersonnelTable->setModel(special_peronnel_model_);
}

ControlWindow::~ControlWindow() {
    std::cout << "~ControlWindow()" << std::endl;
    emit closeSignal();
    delete ui;
}

void ControlWindow::checkFile() {
    QDir dir;

    if (!dir.exists(data_path_.c_str())) {
        bool res = dir.mkpath(data_path_.c_str());
        if (!res) {
            QMessageBox::warning(this, "错误", "data文件不存在，且创建失败");
        }
    }

    QFile community_person_file(community_personnel_file_.c_str());
    if (!community_person_file.open(QIODevice::ReadOnly)) {

        if (!community_person_file.open(QIODevice::WriteOnly)) {
            QMessageBox::warning(this, "错误", "社区人员信息文件不存在，且创建失败");
        }
    }
    community_person_file.close();

    QFile special_person_file(special_peronnel_file_.c_str());
    if (!special_person_file.open(QIODevice::ReadOnly)) {

        if (!special_person_file.open(QIODevice::WriteOnly)) {
            QMessageBox::warning(this, "错误", "特殊人员信息文件不存在，且创建失败");
        }
    }
    special_person_file.close();

    QFile passing_info_file(passing_info_file_.c_str());
    if (!passing_info_file.open(QIODevice::ReadOnly)) {

        if (!passing_info_file.open(QIODevice::WriteOnly)) {
            QMessageBox::warning(this, "错误", "通行信息文件不存在，且创建失败");
        }
    }
    special_person_file.close();

    QFile exception_case_file(exception_case_file_.c_str());
    if (!exception_case_file.open(QIODevice::ReadOnly)) {

        if (!exception_case_file.open(QIODevice::WriteOnly)) {
            QMessageBox::warning(this, "错误", "特殊信息文件不存在，且创建失败");
        }
    }
    exception_case_file.close();
}

void ControlWindow::updateTable() {
    updateCommunityPersonnelModel();
}

void ControlWindow::updateCommunityPersonnelModel() {
    community_personnel_model_->clear();

    QFile file(community_personnel_file_.c_str());
    file.open(QIODevice::ReadWrite);
    QByteArray data = file.readAll();

    //使用json文件对象加载字符串
    QJsonDocument json_doc = QJsonDocument::fromJson(data);

    QJsonArray json_array = json_doc.array();

    int size = json_array.size();
    for (int i = 0; i < size; i++) {
        community_personnel_model_->setItem(i, 1, new QStandardItem(json_array.at(i)["id"].toString()));
        community_personnel_model_->setItem(i, 2, new QStandardItem(json_array.at(i)["name"].toString()));
        community_personnel_model_->setItem(i, 3, new QStandardItem(json_array.at(i)["image_path"].toString()));

    }

    file.close();
}

void ControlWindow::addCommunityPersonData(QString id, QString name, QString image_path) {
    QFile file(community_personnel_file_.c_str());
    file.open(QIODevice::ReadWrite);
    QByteArray data = file.readAll();

    //使用json文件对象加载字符串
    QJsonDocument json_doc = QJsonDocument::fromJson(data);

    QJsonArray json_array = json_doc.array();

    QJsonObject person_obj;
    person_obj.insert("id", id);
    person_obj.insert("name", name);
    person_obj.insert("image_path", image_path);

    json_array.push_back(person_obj);

    json_doc.setArray(json_array);

    file.reset();
    file.write(json_doc.toJson());
    file.close();

    int rows = community_personnel_model_->rowCount();
    community_personnel_model_->setItem(rows, 1, new QStandardItem(id));
    community_personnel_model_->setItem(rows, 2, new QStandardItem(name));
    community_personnel_model_->setItem(rows, 3, new QStandardItem(image_path));

}

void ControlWindow::on_AddCommunityPersonButton_clicked() {
    QString id = ui->CommunityPersonID->text();
    QString name = ui->CommunityPersonName->text();
    QString image_path = ui->CommunityPersonImage->text();

    addCommunityPersonData(id, name, image_path);
}

void ControlWindow::addSpecialPeronData(QString id, QString name, QString remark, QString image_path) {
    QFile file(special_peronnel_file_.c_str());
    file.open(QIODevice::ReadWrite);
    QByteArray data = file.readAll();

    //使用json文件对象加载字符串
    QJsonDocument json_doc = QJsonDocument::fromJson(data);

    QJsonArray json_array = json_doc.array();

    QJsonObject special_person_obj;
    special_person_obj.insert("id", id);
    special_person_obj.insert("name", name);
    special_person_obj.insert("remark", remark);
    special_person_obj.insert("image_path", image_path);

    json_array.push_back(special_person_obj);

    json_doc.setArray(json_array);

    file.reset();
    file.write(json_doc.toJson());
    file.close();

    int rows = special_peronnel_model_->rowCount();
    special_peronnel_model_->setItem(rows, 1, new QStandardItem(id));
    special_peronnel_model_->setItem(rows, 2, new QStandardItem(name));
    special_peronnel_model_->setItem(rows, 3, new QStandardItem(remark));
    special_peronnel_model_->setItem(rows, 4, new QStandardItem(image_path));
}

void ControlWindow::on_SpecialPersonAddButton_clicked() {
    QString id = ui->SpecialPersonId->text();
    QString name = ui->SpecialPersonName->text();
    QString remark = ui->SPecialPersonRemark->text();
    QString image_path = ui->SpecialPersonImagePath->text();

    addSpecialPeronData(id, name, remark, image_path);
}
