//
// Created by idk on 2022/6/9.
//

#ifndef CUSTOMIZE_OCR_FILES_MANAGER_H
#define CUSTOMIZE_OCR_FILES_MANAGER_H

#include <string>
#include <QFileInfo>
#include <QString>
#include <QDir>
#include <iostream>

namespace Utility {
    namespace Files_Manager {

        QString path_combine(const QString &path1, const QString &path2);

        bool is_exists_any(const std::string &path);
        bool is_exists_file(const std::string &path);
        bool is_exists_folder(const std::string &path);

        bool mkdir(const std::string &path, const std::string &folder_name);
//        bool mkdir(const std::string &absolute_folder_path);

        std::string absolute_path_join(const std::string& absolute_path, const std::string& join_path);
    };
};


#endif //CUSTOMIZE_OCR_FILES_MANAGER_H
