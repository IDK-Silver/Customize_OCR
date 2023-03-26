//
// Created by idk on 2022/10/2.
//

#ifndef CUSTOMIZE_IMAGE_FORMAT
#define CUSTOMIZE_IMAGE_FORMAT
#include <opencv2/opencv.hpp>
#include <QDebug>
#include <QImage>
#include <QPixmap>
#include <leptonica/allheaders.h>

namespace Utility {

    cv::Mat QImage2cvMat(QImage image);
    QPixmap Mat2QPixmap(cv::Mat mat);
    Pix* QImage2Pix(QImage qImage);
    QImage Pix2QImage(Pix* pixImage);

    cv::Mat MatCropByQRect(const cv::Mat&, QRect);

} // Utility

#endif //CUSTOMIZE_IMAGE_FORMAT
