//
// Created by idk on 2022/6/9.
//

#include "Global_Config.h"

namespace Config::Global {
        const std::string application_name = "Customize_OCR";
        const std::string config_name = "config";
        extern const std::string bin_folder_name = "bin";
        extern const std::string format_setting_folder_name = "formats";
        bool is_output_log_message = true;
        extern const std::string ocr_model_folder_name="model";
        extern const bool is_output_io_error_message = true;
};
