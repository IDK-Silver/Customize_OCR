//
// Created by idk on 2022/6/8.
//

#include "Files_Path.h"
#include <QDir>
#include <lib/Config/Global_Config/Global_Config.h>
#include <lib/Utility/Files_Manager/Files_Manager.h>

namespace Config::Files_Path {

        std::string get_config_path()
        {
            auto AppDataLocation = QStandardPaths::standardLocations(QStandardPaths::AppDataLocation);
            auto system_app_config_path = AppDataLocation.at(0).toLocal8Bit();
            QDir path(system_app_config_path);

            // det application path is exists
            if (!path.cd(QString::fromStdString(Config::Global::application_name)))
            {
                // create folder
                Utility::Files_Manager::mkdir(path.absolutePath().toLocal8Bit().toStdString(), Config::Global::application_name);
                path.cd(QString::fromStdString(Config::Global::application_name));
            }

            // det config path is exists
            if (!path.cd(QString::fromStdString(Config::Global::config_name)))
            {
                // create folder
                Utility::Files_Manager::mkdir(path.absolutePath().toLocal8Bit().toStdString(), Config::Global::config_name);
                path.cd(QString::fromStdString(Config::Global::config_name));
            }



            return path.absolutePath().toLocal8Bit().toStdString();
        }
    }

