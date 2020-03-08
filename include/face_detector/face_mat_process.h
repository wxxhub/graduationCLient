#ifndef FACEMATPROCESS_H
#define FACEMATPROCESS_H

#include <opencv2/opencv.hpp>

#include "mtnn/mtcnn.hpp"

const int detector_width = 100;

void getDetectorMat(const cv::Mat &image, cv::Mat &detector_image, const face_box &box) {
    int width = (box.x1 - box.x0) > (box.y1 - box.y0) ? (box.x1 - box.x0) : (box.y1 - box.y0);
    width = width > detector_width ?  width : detector_width;
    int x_centre = (box.x1 + box.x0)/2;
    int y_centre = (box.y1 + box.y0)/2;

    if (x_centre < width / 2) {
        x_centre = width / 2;
    } else if (image.cols - x_centre < width/2) {
        x_centre = image.cols - (width/2);
    }

    if (y_centre < width/2) {
        y_centre = width/2;
    } else if (image.rows - y_centre < width/2) {
        y_centre =image.rows - (width/2);
    }

    int start_x = x_centre - (width / 2);
    int start_y = y_centre - (width / 2);
    // cout << start_x << ":" << start_y << ":" << x_centre << ":" << y_centre << ":" << box.x0 << endl;
    cv::Mat cut_image = image(cv::Rect(start_x,  start_y, width, width));

    resize(cut_image, cut_image, cv::Size(detector_width, detector_width));
    cvtColor(cut_image, cut_image, cv::COLOR_BGR2GRAY);
    detector_image = cut_image.clone();
}

#endif // FACEMATPROCESS_H
