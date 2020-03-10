#include <fstream>

#include "face_detector/identity_authencation.h"

using namespace std;
using namespace cv;

IdentityAuthencation::IdentityAuthencation()
    : K_(1)
    , detector_width_(100) {

}

bool  IdentityAuthencation::init(const std::string &model_path, const std::string &label_path) {
    ifstream ifs;
    ifs.open(model_path, ios::in);

    if (!ifs.is_open()) {
        cout << "model_path open failed！" << endl;
        return false;
    }

    detector_model_ = ml::StatModel::load<ml::KNearest>(model_path);

    return getLabels(label_path);
}

bool IdentityAuthencation::getLabels(const std::string &label_path) {
    ifstream ifs;
    ifs.open(label_path, ios::in);

    if (!ifs.is_open()) {
        cout << "label_path open failed！" << endl;
        return false;
    }

    char buf[64];
    while (ifs.getline(buf, sizeof(buf))) {
        labels_.push_back(buf);
    }

    return true;
}

string IdentityAuthencation::detector(cv::Mat &image) {
    Mat predict_mat = image.reshape(0, 1);
    predict_mat.convertTo(predict_mat, CV_32F);

    int predict_y = detector_model_->predict(predict_mat);

    return labels_[predict_y];
}
