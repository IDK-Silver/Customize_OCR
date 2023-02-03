//
// Created by idk on 2022/6/8.
//

#include "Files_Path.h"
#include <Utility/Files_Manager/Files_Manager.h>
#include <QDir>
#include <Config/Global_Config/Global_Config.h>


namespace Config::Files_Path {

    void det_path(QDir& path, const std::string& add_path) {
        // det path is exists
        if (!path.cd(QString::fromStdString(add_path)))
        {
            // create folder
            Utility::Files_Manager::mkdir(path.absolutePath().toLocal8Bit().toStdString(), add_path);
            path.cd(QString::fromStdString(add_path));
        }
    }

    std::string get_config_path()
    {
        QDir config_path = QString::fromStdString(app_data_location());
        QDir path(config_path);

        det_path(path, Config::Global::application_name);
        det_path(path, Config::Global::config_name);
        return path.absolutePath().toLocal8Bit().toStdString();
    }


    std::string app_data_location() {
        auto AppDataLocation = QStandardPaths::standardLocations(QStandardPaths::AppDataLocation);
        auto system_app_config_path = AppDataLocation.at(0).toLocal8Bit();
        QDir path(system_app_config_path);
        return path.absolutePath().toLocal8Bit().toStdString();
    }

    std::string get_format_setting_folder_path() {
        QDir config_path = QString::fromStdString(app_data_location());
        QDir path(config_path);
        det_path(path, Config::Global::format_setting_folder_name);
        return path.absolutePath().toLocal8Bit().toStdString();
    }

    std::string get_system_picture_path() {
        QStringList PicturesLocation = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
        return PicturesLocation.at(0).toLocal8Bit().toStdString();
    }

    std::string get_ocr_model_path() {

        QDir ocr_model_path = QDir::currentPath();

        ocr_model_path = Utility::Files_Manager::path_combine(ocr_model_path.absolutePath(),
                                                              QString::fromStdString(Global::ocr_model_folder_name));
        return ocr_model_path.absolutePath().toLocal8Bit().toStdString();
    }
}

