#include "../../include/image_process/image_process.h"

using namespace tf_image_process;
using namespace cv;
using namespace std;

string class_names_[] = {"background", "person", "bicycle", "car", "motorcycle", "airplane", "bus", "train", "truck", "boat", "traffic light",
        "fire hydrant", "background", "stop sign", "parking meter", "bench", "bird", "cat", "dog", "horse", "sheep", "cow", "elephant", "bear", "zebra", "giraffe", "background", "backpack",
        "umbrella", "background", "background", "handbag", "tie", "suitcase", "frisbee","skis", "snowboard", "sports ball", "kite", "baseball bat","baseball glove", "skateboard", "surfboard", "tennis racket",
        "bottle", "background", "wine glass", "cup", "fork", "knife", "spoon","bowl", "banana",  "apple", "sandwich", "orange","broccoli", "carrot", "hot dog",  "pizza", "donut",
        "cake", "chair", "couch", "potted plant", "bed", "background", "dining table", "background", "background", "toilet", "background","tv", "laptop", "mouse", "remote", "keyboard",
        "cell phone", "microwave", "oven", "toaster", "sink", "refrigerator", "background","book", "clock", "vase", "scissors","teddy bear", "hair drier", "toothbrush"};

ImageProcess::ImageProcess(const std::string &weight, const std::string &prototxt)
    : in_width_(300)
    , in_height_(300)
    , wh_ratio_(in_width_ / (float)in_height_)
    , weights_(weight)
    , prototxt_(prototxt) {

    net_ = cv::dnn::readNetFromTensorflow(weights_, prototxt_);
}

Mat ImageProcess::process(Mat image) {
    Size image_size = image.size();
    Size cropSize;
    if (image_size.width / (float)image_size.height > wh_ratio_)
    {
        cropSize = Size(static_cast<int>(image_size.height * wh_ratio_),
            image_size.height);
    }
    else
    {
        cropSize = Size(image_size.width,
            static_cast<int>(image_size.width / wh_ratio_));
    }

    Rect crop(Point((image_size.width - cropSize.width) / 2,
        (image_size.height - cropSize.height) / 2),
        cropSize);


    cv::Mat blob = cv::dnn::blobFromImage(image, 1. / 255, Size(300, 300));
    //cout << "blob size: " << blob.size << endl;

    net_.setInput(blob);
    Mat output = net_.forward();
    //cout << "output size: " << output.size << endl;

    Mat detectionMat(output.size[2], output.size[3], CV_32F, output.ptr<float>());

    image = image(crop);
    float confidenceThreshold = 0.20;
    for (int i = 0; i < detectionMat.rows; i++)
    {
        float confidence = detectionMat.at<float>(i, 2);

        if (confidence > confidenceThreshold)
        {
            size_t objectClass = (size_t)(detectionMat.at<float>(i, 1));

            int xLeftBottom = static_cast<int>(detectionMat.at<float>(i, 3) * image.cols);
            int yLeftBottom = static_cast<int>(detectionMat.at<float>(i, 4) * image.rows);
            int xRightTop = static_cast<int>(detectionMat.at<float>(i, 5) * image.cols);
            int yRightTop = static_cast<int>(detectionMat.at<float>(i, 6) * image.rows);

            ostringstream ss;
            ss << confidence;
            String conf(ss.str());

            Rect object((int)xLeftBottom, (int)yLeftBottom,
                (int)(xRightTop - xLeftBottom),
                (int)(yRightTop - yLeftBottom));

            rectangle(image, object, Scalar(0, 255, 0), 2);
            String label = String(class_names_[objectClass]) + ": " + conf;
            int baseLine = 0;
            Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
            rectangle(image, Rect(Point(xLeftBottom, yLeftBottom - labelSize.height),
                Size(labelSize.width, labelSize.height + baseLine)),
                Scalar(0, 255, 0), -1);
            putText(image, label, Point(xLeftBottom, yLeftBottom),
                FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0));
        }
    }
    imshow("image", image);

    return image;
}
