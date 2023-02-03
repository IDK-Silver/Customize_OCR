//
// Created by idk on 2023/1/17.
//

#ifndef CUSTOMIZE_OCR_WIDGET_CONFIG_H
#define CUSTOMIZE_OCR_WIDGET_CONFIG_H

#include "../Files_Path/Files_Path.h"
#include <Utility/Files_Manager/Files_Manager.h>
#include <Config/Global_Config/Global_Config.h>
#include <QString>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <iostream>

namespace Config::AppConfig {

    const QString FileName = "app_config.json";
    extern const QString FilePath;

    void file_generation();

    class OCRDetectWidgetConfig {
        public:
            // json struct
            const QString Key = "OCR Detect Widget";
            struct __XlsxOption__ {
                const QString Key = "Xlsx Option";

                struct __WriteMode__ {
                    const QString Key = "write mode";
                    const QString add = "add";
                    const QString replace = "replace";
                };
                __WriteMode__ WriteMode;
            };
            __XlsxOption__ XlsxOption;


            OCRDetectWidgetConfig();
            ~OCRDetectWidgetConfig();

            QString get_xlsx_write_mode();
            void set_xlsx_write_mode(const QString& mode);

        private:
            QJsonDocument json_doc;
    };
}


#endif //CUSTOMIZE_OCR_WIDGET_CONFIG_H
