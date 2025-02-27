#ifndef CONTROL_WINDOW_H
#define CONTROL_WINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>

#include <set>

#include "define.h"

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

    std::string data_path_;
    std::string community_personnel_file_;
    std::string special_peronnel_file_;
    std::string passing_info_file_;
    std::string exception_case_file_;

    QStandardItemModel *community_personnel_model_;
    QStandardItemModel *special_peronnel_model_;
    QStandardItemModel *passing_info_model_;
    QStandardItemModel *exception_case_model_;

    void checkFile(); // check file, if no file, create it.
    void updateTable(); // update table.

    void updateCommunityPersonnelModel();
    void addCommunityPersonData(QString id, QString name, QString image_path);

    void updateSpecialPersonModel();
    void addSpecialPeronData(QString id, QString name, QString remark, QString image_path);

    void updatePassingInfoModel();
    void updateExceptionCaseModel();

    void deletePassingInfo(std::set<std::string> &ids);
    void deleteExceptionCase(std::set<std::string> &ids);
signals:
    void closeSignal();
private slots:
    void on_AddCommunityPersonButton_clicked();
    void on_SpecialPersonAddButton_clicked();
    void on_PassingInfoUpdateButton_clicked();
    void on_ExceptionCaseUpdateButton_clicked();
    void on_SpecialPersonUpdateButton_clicked();
    void on_CommunityPersonUpdateButton_clicked();
    void on_SetCommunityPersonImageButton_clicked();
    void on_SpecialPersonImageButton_clicked();
    void on_CommunityPersonDeleteButton_clicked();
    void on_CommunityPersonSelectAllButton_clicked();
    void on_SpecialPersonDeleteButton_clicked();
    void on_SpecialPersonSelectButton_clicked();
};

#endif // CONTROL_WINDOW_H
