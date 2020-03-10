#include "face_detector/face_detector.h"

using namespace std;
using namespace cv;

FaceDetector::FaceDetector()
    : detector_width_(100)
    , K_(1) {


}

FaceDetector::~FaceDetector() {
    TF_Status* s = TF_NewStatus();

    TF_CloseSession(sess_,s);
    TF_DeleteSession(sess_,s);
    TF_DeleteGraph(graph_);
    TF_DeleteStatus(s);
}

bool FaceDetector::init(const string &model_path) {
    sess_ = load_graph(model_path.c_str(),&graph_);

    return true;
}

std::vector<face_box> FaceDetector::detector(cv::Mat &image) {
    vector<face_box> face_info;

    mtcnn_detect(sess_, graph_, image,face_info);

    return  face_info;
}

void FaceDetector::drawBox(cv::Mat &image, std::vector<face_box> &boxs) {
    for(unsigned int i = 0; i < boxs.size(); i++) {
        face_box& box = boxs[i];

        /*draw box */
        rectangle(image, Point(box.x0, box.y0), Point(box.x1, box.y1), Scalar(0, 255, 0), 1);

        for(int l=0;l<5;l++) {
            circle(image,Point(box.landmark.x[l],box.landmark.y[l]),1,Scalar(0, 0, 255),2);
        }
    }
}
