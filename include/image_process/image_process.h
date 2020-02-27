#ifndef IMAGEPROCESS_H
#define IMAGEPROCESS_H

#include "opencv2/opencv.hpp"
#include "opencv2/dnn.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/types_c.h"

namespace tf_image_process {


class ImageProcess {
public:
    ImageProcess(const std::string &weight, const std::string &prototxt);

    cv::Mat process(cv::Mat image);

private:
    const size_t in_width_;
    const size_t in_height_;
    const float wh_ratio_;

    const std::string weights_;
    const std::string prototxt_;

    cv::dnn::Net net_;
};

}

#endif // IMAGEPROCESS_H
