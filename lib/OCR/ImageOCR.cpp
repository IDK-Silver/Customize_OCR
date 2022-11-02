//
// Created by idk on 2022/10/2.
//

#include "ImageOCR.h"

ImageOCR::ImageOCR() {
    /* new TessBaseAPI */
    this->tesseract_api = std::make_unique<tesseract::TessBaseAPI>();

    /* tesseract api init and load model */
    this->tesseract_api->Init(Config::Files_Path::get_ocr_model_path().c_str(), "chi_tra+eng");
}

ImageOCR::~ImageOCR() = default;

void ImageOCR::set_image(cv::Mat input_image)
{
    /* image format to gray */
    cv::cvtColor(input_image, input_image, cv::COLOR_BGR2GRAY);

    #if (ProgramTest)
        cv::imwrite("./ocr_gray_image.jpg", input_image);
    #endif

    /* image adaptive threshold*/
    cv::adaptiveThreshold(input_image, input_image, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY_INV, 15, 2);

    #if (ProgramTest)
        cv::imwrite("./ocr_adaptive_threshold_image.jpg", input_image);
    #endif

    /* load */
    this->tesseract_api->SetImage(input_image.data, input_image.cols, input_image.rows,
                                  1, input_image.cols);
}

QString ImageOCR::ocr() {
    return tesseract_api->GetUTF8Text();
}