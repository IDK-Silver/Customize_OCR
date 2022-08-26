//
// Created by idk on 2022/8/22.
//

#include "ocr_format_struct.h"

#include <utility>

OCR_Format_Setting::OCR_Format_Setting(QString setting_name, QImage image, std::vector<std::shared_ptr<OCR_Format_Data>> format_list) {
    this->settting_name = std::move(setting_name);
    this->image = std::move(image);
    this->format_list = std::move(format_list);
}

bool OCR_Format_Setting::save_file(QString save_path) {


    return true;
}

OCR_Format_Setting::~OCR_Format_Setting() = default;
