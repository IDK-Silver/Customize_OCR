//
// Created by idk on 2022/10/2.
//

#ifndef CUSTOMIZE_OCR_QT_CV_H
#define CUSTOMIZE_OCR_QT_CV_H

#include <opencv2/opencv.hpp>
#include <QImage>
#include <QPixmap>

namespace Utility {

    class Qt_CV {
        cv::Mat Qt_CV::QImage2cvMat(QImage image);
        QPixmap Qt_CV::Mat2QPixmap(cv::Mat mat);
    };

} // Utility

#endif //CUSTOMIZE_OCR_QT_CV_H
