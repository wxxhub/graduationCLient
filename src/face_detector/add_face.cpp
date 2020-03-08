#include <iostream>
#include <dirent.h>

#include "face_detector/add_face.h"
#include "face_detector/face_mat_process.h"

using namespace std;
using namespace cv;

static int getPDistance(int x1, int x2, int y1, int y2) {
    return sqrt(powf(x1 - x2, 2) + powf(y1 - y2, 2));
}

AddFace::AddFace()
    : save_index_(0)
    , min_same_(3) {

}

FACE_STATUS AddFace::getFaceStatus(const face_box &box) {
    int d1 = getPDistance(box.landmark.x[0], box.landmark.x[2], box.landmark.y[0], box.landmark.y[2]);
    int d2 = getPDistance(box.landmark.x[1], box.landmark.x[2], box.landmark.y[1], box.landmark.y[2]);
    int d3 = getPDistance(box.landmark.x[3], box.landmark.x[2], box.landmark.y[3], box.landmark.y[2]);
    int d4 = getPDistance(box.landmark.x[4], box.landmark.x[2], box.landmark.y[4], box.landmark.y[2]);
    int average_d = (d1 + d2 + d3 + d4) / 4;

    double 	scale1 = 1.0 * d1 / average_d;
    double 	scale2 = 1.0 * d2 / average_d;
    double 	scale3 = 1.0 * d3 / average_d;
    double 	scale4 = 1.0 * d4 / average_d;
    // cout << setw( 15 ) << scale1 << ";" << setw( 15 ) << scale2 << ";" << setw( 15 ) << scale3 << ";" << setw( 15 ) << scale4 << endl;

    if (scale1 < 0.9 && scale3 < 0.9) {
        return LEFT;
    } else if (scale2 < 0.9 && scale4 < 0.9) {
        return RIGHT;
    }

    return CENTER;
}

bool AddFace::init(const string &model_path) {
    reset();

    for (int i = 0; i < 8; i++) {
        face_mats_[i] = Mat(detector_width, detector_width, CV_8UC3, Scalar(255, 255, 255));
    }

    face_detector_ = new FaceDetector();
    return face_detector_->init(model_path);
}

bool AddFace::process(cv::Mat &image) {
    vector<face_box> boxs = face_detector_->detector(image);

    if (boxs.size() > 0 && save_index_ < 8) {
        FACE_STATUS current_status = getFaceStatus(boxs[0]);
        if (current_status == last_status_) {
            same_status_times_++;
        } else {
            last_status_ = current_status;
            same_status_times_ = 0;
        }

        if (same_status_times_ > min_same_ && current_status == save_logical_[save_index_]) {
            getDetectorMat(image, face_mats_[save_index_], boxs[0]);
            save_index_++;
            return true;
        }
    }

    return false;
}

cv::Mat AddFace::getCurrentImage(bool &finish) {
    if (save_index_ == 8) {
        finish = true;
    } else {
        finish = false;
    }

    cout << save_index_ << endl;
    return face_mats_[save_index_-1];
}

SAVE_STATUS AddFace::save() {
    string path = save_path_ + name_;

    char image_file[128];

    for (int i = 0; i < 8; i++) {
        sprintf(image_file, "%s/%s_%d.jpg", path.c_str(), name_.c_str(), i);
        imwrite(image_file, face_mats_[i]);
    }

    return SAVE_SUCCESS;
}

void AddFace::reset() {
    save_index_ = 0;
    same_status_times_ = 0;
    last_status_ = CENTER;
}

void AddFace::setSave(const std::string &save_path, const std::string &name) {
    if (save_path[save_path.size() - 1] != '/') {
        save_path_ = save_path + '/';
    } else {
        save_path_ = save_path;
    }

    name_ = name;
}
