#ifndef FACEMATPROCESS_H
#define FACEMATPROCESS_H

#include <opencv2/opencv.hpp>

#include "mtnn/mtcnn.hpp"

const int detector_width = 100;

void getDetectorMat(const cv::Mat &image, cv::Mat &detector_image, const face_box &box);

#endif // FACEMATPROCESS_H
