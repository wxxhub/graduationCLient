#ifndef FACEDETECTOR_H
#define FACEDETECTOR_H

#include <fstream>
#include <utility>
#include <vector>
#include <opencv2/ml/ml.hpp>
#include <opencv2/ml.hpp>

#include "tensorflow/c/c_api.h"
#include "mtnn/tensorflow_mtcnn.hpp"
#include "mtnn/mtcnn.hpp"
#include "mtnn/comm_lib.hpp"
#include "mtnn/utils.hpp"

class FaceDetector {
public:
    FaceDetector();
    ~FaceDetector();

    bool init(const std::string &model_path);
    std::vector<face_box> detector(cv::Mat &image);

private:
    const int detector_width_;
    const int K_;

    TF_Session *sess_;
    TF_Graph *graph_;

    cv::Ptr<cv::ml::KNearest> detector_model_;
};

#endif // FACEDETECTOR_H
