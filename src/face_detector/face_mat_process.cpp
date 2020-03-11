#include "face_detector/face_mat_process.h"

void getDetectorMat(const cv::Mat &image, cv::Mat &detector_image, const face_box &box) {
    int width = (box.x1 - box.x0) > (box.y1 - box.y0) ? (box.x1 - box.x0) : (box.y1 - box.y0);

    if (width > image.cols) {
        width = image.cols;
    }

    if (width > image.rows) {
        width = image.rows;
    }

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

    // avoid out of range.
    if (start_y + width > image.rows) {
        start_y--;
    }
    if (start_x + width > image.cols) {
        start_x--;
    }
    if (start_y < 0) {
        start_y = 0;
    }
    if (start_x < 0) {
        start_x = 0;
    }

    cv::Mat cut_image = image(cv::Rect(start_x,  start_y, width, width));

    resize(cut_image, cut_image, cv::Size(detector_width, detector_width));
    cvtColor(cut_image, cut_image, cv::COLOR_BGR2GRAY);
    detector_image = cut_image.clone();
}
