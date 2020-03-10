#ifndef IDENTITYAUTHENCATION_H
#define IDENTITYAUTHENCATION_H

#include <iostream>
#include <vector>

#include <opencv2/ml/ml.hpp>
#include <opencv2/ml.hpp>


class IdentityAuthencation {
public:
    IdentityAuthencation();

    bool init(const std::string &model_path, const std::string &label_path);
    std::string detector(cv::Mat &image);

private:
    const int K_;
    const int detector_width_;

    cv::Ptr<cv::ml::KNearest> detector_model_;

    std::vector<std::string> labels_;

    bool getLabels(const std::string &label_path);
};

#endif // IDENTITYAUTHENCATION_H
