//
// Created by idk on 2022/10/2.
//

#ifndef CUSTOMIZE_OCR_IMAGE_OCR
#define CUSTOMIZE_OCR_IMAGE_OCR
#include <vector>
#include <memory>
#include <opencv2/opencv.hpp>
#include <leptonica/allheaders.h>
#include <tesseract/baseapi.h>
#include <QImage>
#include <Config/Files_Path/Files_Path.h>
#include <Config/Global_Config/Global_Config.h>

class ImageOCR {
public:
    ImageOCR();
    ~ImageOCR();

    void set_image(cv::Mat input_image);
    QString ocr();
private:
    std::unique_ptr<tesseract::TessBaseAPI> tesseract_api;

};


#endif //CUSTOMIZE_OCR_IMAGE_OCR
