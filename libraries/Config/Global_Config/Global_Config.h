//
// Created by idk on 2022/6/9.
//

#ifndef CUSTOMIZE_OCR_GLOBAL_CONFIG_H
#define CUSTOMIZE_OCR_GLOBAL_CONFIG_H

#include <string>

namespace Config::Global {
    #define ProgramTest 1
    extern const std::string bin_folder_name;
    extern const std::string application_name;
    extern const std::string config_name;
    extern const std::string format_setting_folder_name;
    extern const std::string ocr_model_folder_name;
    extern bool is_output_log_message;
    extern const bool is_output_io_error_message;
};


#endif //CUSTOMIZE_OCR_GLOBAL_CONFIG_H
