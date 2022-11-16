//
// Created by idk on 2022/6/8.
//

#include <string>
#include <QStandardPaths>
#include <Utility/Files_Manager/Files_Manager.h>

#ifndef CUSTOMIZE_OCR_FILES_PATH_H
#define CUSTOMIZE_OCR_FILES_PATH_H

namespace Config::Files_Path {
        std::string get_system_picture_path();
        std::string get_config_path();
        std::string app_data_location();
        std::string get_format_setting_folder_path();
        std::string get_ocr_model_path();

}

#endif //CUSTOMIZE_OCR_FILES_PATH_H
