//
// Created by idk on 2022/10/2.
//

#ifndef PPOCR_IMAGEOCR_H
#define PPOCR_IMAGEOCR_H
#include <opencv2/opencv.hpp>
#include <vector>
#include <QImage>

class ImageOCR {
public:
    ImageOCR();
    ~ImageOCR();

    bool add_ocr_image(QImage image);
private:

};


#endif //PPOCR_IMAGEOCR_H
