//
// Created by idk on 2022/6/9.
//

#include <QFileInfo>
#include <QString>
#include <QDir>
#include <iostream>
#include "Files_Manager.h"

using namespace Utility;

bool Files_Manager::is_exists_any(const std::string& path) {
    QFileInfo check_file(QString::fromStdString(path));
    // check if file exists and if yes : Is it really a file and no directory ?
    if (check_file.exists()) {
        return true;
    } else {
        return false;
    }
};

bool Files_Manager::is_exists_file(const std::string &path) {
    QFileInfo check_file(QString::fromStdString(path));
    if (check_file.exists() && check_file.isFile()) {
        return true;
    } else {
        return false;
    }
}

bool Files_Manager::is_exists_folder(const std::string &path) {
    QFileInfo check_file(QString::fromStdString(path));
    if (check_file.exists() && (!check_file.isFile())) {
        return true;
    } else {
        return false;
    }
}

// 不保證路徑存在的連接方式
std::string Files_Manager::absolute_path_join(const std::string& absolute_path, const std::string& join_path) {

    return QString::fromStdString(absolute_path).append(QDir::separator()).append(QString::fromStdString(join_path)).toLocal8Bit().toStdString();
}

bool Files_Manager::mkdir(const std::string &path, const std::string &folder_name) {

    if (is_exists_folder(absolute_path_join(path, folder_name)))
        return true;

    QDir q_dir(QString::fromStdString(path));

    std::cout << "Create Folder : " << folder_name << ", At : " << path << std::endl;

    return q_dir.mkdir(QString::fromStdString(folder_name));
}
