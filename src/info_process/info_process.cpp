#include "info_process/info_process.h"

#include <QCoreApplication>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QTime>

#include <iostream>

using namespace std;

InfoProcess::InfoProcess() {
    init();
}

void InfoProcess::init() {
    // init path
    string applition_path =  QCoreApplication::applicationDirPath().toStdString();
    community_personnel_data_file_ = applition_path +"/data/community_personnel.json";
    special_personnel_data_file_ = applition_path +"/data/special_peronnel.json";

    update();
}

void InfoProcess::update() {
    loadCommunityPersonnelData();
    loadSpecialPersonnelData();
}

PersonInfo InfoProcess::find(std::string id) {
    if (id == "unknown") {
        return UNKNOWN_PERSON;
    }

    if (special_personnel_set_.find(id) != special_personnel_set_.end()) {
        return SPECIAL_PERSON;
    }

    if (community_personnel_set_.find(id) != community_personnel_set_.end()) {
        return COMMUNITY_PERSON;
    }

    return UNKNOWN_PERSON;
}

void InfoProcess::loadCommunityPersonnelData() {
    community_personnel_set_.clear();
    community_person_map_.clear();

    QFile file(community_personnel_data_file_.c_str());
    file.open(QIODevice::ReadWrite);
    QByteArray data = file.readAll();

    //使用json文件对象加载字符串
    QJsonDocument json_doc = QJsonDocument::fromJson(data);

    QJsonArray json_array = json_doc.array();

    int size = json_array.size();
    for (int i = 0; i < size; i++) {
        Person person;
        person.id = json_array.at(i)["id"].toString().toStdString();
        person.name = json_array.at(i)["name"].toString().toStdString();
        person.head_image_path = json_array.at(i)["image_path"].toString().toStdString();
        community_person_map_[person.id] = person;
        community_personnel_set_.insert(person.id);
    }

    file.close();
}

void InfoProcess::loadSpecialPersonnelData() {
    special_personnel_set_.clear();
    special_person_map_.clear();

    QFile file(special_personnel_data_file_.c_str());
    file.open(QIODevice::ReadWrite);
    QByteArray data = file.readAll();

    //使用json文件对象加载字符串
    QJsonDocument json_doc = QJsonDocument::fromJson(data);

    QJsonArray json_array = json_doc.array();

    int size = json_array.size();
    for (int i = 0; i < size; i++) {
        Person person;
        person.id = json_array.at(i)["id"].toString().toStdString();
        person.name = json_array.at(i)["name"].toString().toStdString();
        person.head_image_path = json_array.at(i)["image_path"].toString().toStdString();

        SpecialPerson special_person;
        special_person.person = person;
        special_person.remark = json_array.at(i)["remark"].toString().toStdString();

        special_person_map_[special_person.person.id] = special_person;
        special_personnel_set_.insert(special_person.person.id);
    }

    file.close();
}

Person InfoProcess::getPerson(std::string id) {
    return community_person_map_[id];
}

SpecialPerson InfoProcess::getSpecialPerson(std::string id) {
    return special_person_map_[id];
}
