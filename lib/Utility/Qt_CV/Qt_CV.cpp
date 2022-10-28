//
// Created by idk on 2022/10/2.
//

#include "Qt_CV.h"

namespace Utility {
    cv::Mat Qt_CV::QImage2cvMat(QImage image)
    {
        cv::Mat mat;
        //qDebug() << image.format();
        switch (image.format())
        {
            case QImage::Format_ARGB32:
            case QImage::Format_RGB32:
            case QImage::Format_ARGB32_Premultiplied:
                {
                    cv::Mat mat_temp = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
                    cvtColor(mat_temp, mat, cv::COLOR_BGRA2BGR);
                }
                break;

            case QImage::Format_RGB888:
                mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
                break;

            case QImage::Format_Indexed8:
                mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
                break;

            default:
                break;
        }
        return mat;
    }

    QPixmap Qt_CV::Mat2QPixmap(cv::Mat mat)
    {
        QPixmap pixmap;
        if (mat.type() == CV_8UC1)
        {
            // Set the color table (used to translate colour indexes to qRgb values)
            QVector<QRgb> colorTable;
            for (int i = 0; i < 256; i++)
                colorTable.push_back(qRgb(i, i, i));
            // Copy input Mat
            const uchar* qImageBuffer = (const uchar*)mat.data;
            // Create QImage with same dimensions as input Mat
            QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8);
            img.setColorTable(colorTable);
            pixmap = QPixmap::fromImage(img);
        }
        // 8-bits unsigned, NO. OF CHANNELS=3
        if (mat.type() == CV_8UC3)
        {
            // Copy input Mat
            const uchar* qImageBuffer = (const uchar*)mat.data;
            // Create QImage with same dimensions as input Mat
            QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
            pixmap = QPixmap::fromImage(img.rgbSwapped());
        }
        return pixmap;
    }



} // Utility