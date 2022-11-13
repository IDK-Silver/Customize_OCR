//
// Created by idk on 2022/10/2.
//

#include "ImageFormat.h"

namespace Utility {
    cv::Mat QImage2cvMat(QImage image)
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

    QPixmap Mat2QPixmap(cv::Mat mat)
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

    Pix* QImage2Pix(QImage qImage)
    {
        PIX * pixs;
        l_uint32 *lines;

        qImage = qImage.rgbSwapped();
        int width = qImage.width();
        int height = qImage.height();
        int depth = qImage.depth();
        int wpl = qImage.bytesPerLine() / 4;

        pixs = pixCreate(width, height, depth);
        pixSetWpl(pixs, wpl);
        pixSetColormap(pixs, NULL);
        l_uint32 *datas = pixs->data;

        for (int y = 0; y < height; y++) {
            lines = datas + y * wpl;
            QByteArray a((const char*)qImage.scanLine(y), qImage.bytesPerLine());
            for (int j = 0; j < a.size(); j++) {
                *((l_uint8 *)lines + j) = a[j];
            }
        }
        return pixEndianByteSwapNew(pixs);
    }

    QImage Pix2QImage(Pix* pixImage)
    {
        int width = pixGetWidth(pixImage);
        int height = pixGetHeight(pixImage);
        int depth = pixGetDepth(pixImage);
        int bytesPerLine = pixGetWpl(pixImage) * 4;
        l_uint32 * s_data = pixGetData(pixEndianByteSwapNew(pixImage));

        QImage::Format format;
        if (depth == 1)
            format = QImage::Format_Mono;
        else if (depth == 8)
            format = QImage::Format_Indexed8;
        else
            format = QImage::Format_RGB32;

        QImage result((uchar*)s_data, width, height, bytesPerLine, format);

        // Handle pallete
        QVector<QRgb> _bwCT;
        _bwCT.append(qRgb(255,255,255));
        _bwCT.append(qRgb(0,0,0));

        QVector<QRgb> _grayscaleCT(256);
        for (int i = 0; i < 256; i++)  {
            _grayscaleCT.append(qRgb(i, i, i));
        }
        if (depth == 1) {
            result.setColorTable(_bwCT);
        }  else if (depth == 8)  {
            result.setColorTable(_grayscaleCT);

        } else {
            result.setColorTable(_grayscaleCT);
        }

        if (result.isNull()) {
            static QImage none(0,0,QImage::Format_Invalid);
            qDebug() << "***Invalid format!!!";
            return none;
        }

        return result.rgbSwapped();
    }

    cv::Mat MatCropByQRect(const cv::Mat& image, QRect rect)
    {
        auto left = rect.topLeft();
        qDebug() << rect.width() << " " << rect.height();
        auto crop_image = image;
        return crop_image(cv::Rect(left.x(), left.y(), rect.width(), rect.height()));
    }

} // Utility