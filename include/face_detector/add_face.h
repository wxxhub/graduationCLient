#ifndef ADDFACE_H
#define ADDFACE_H

#include "face_detector/face_detector.h"

enum FACE_STATUS {
    CENTER = 0,
    LEFT = 1,
    RIGHT = 2,
    TOP = 3,
    BUTTON = 4,
};

enum SAVE_STATUS {
    EXIST = -1,
    SAVE_FAILE = 0,
    SAVE_SUCCESS = 1,
};

class AddFace
{
public:
    AddFace();

    bool init(const std::string &model_path);
    bool process(cv::Mat &image); // 返回是否更新数据
    SAVE_STATUS save();
    cv::Mat getCurrentImage(bool &finish);

    void reset();   //重置
    void setSave(const std::string &save_path, const std::string &name);
private:
    int save_index_;
    std::string save_path_;
    std::string name_;

    FACE_STATUS save_logical_[8] = {CENTER, LEFT, CENTER, RIGHT, CENTER, LEFT, CENTER, RIGHT};
    cv::Mat face_mats_[8];

    FaceDetector *face_detector_;

    FACE_STATUS getFaceStatus(const face_box &box);

    FACE_STATUS last_status_;
    int same_status_times_;

    const int min_same_;
};

#endif // ADDFACE_H
