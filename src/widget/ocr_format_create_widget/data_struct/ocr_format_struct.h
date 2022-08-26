//
// Created by idk on 2022/8/22.
//

#ifndef CUSTOMIZE_OCR_OCR_FORMAT_STRUCT_H
#define CUSTOMIZE_OCR_OCR_FORMAT_STRUCT_H

#include <QString>
#include <QRect>
#include <qimage.h>
#include <vector>
#include <memory>

class OCR_Format_Data {
public:
    QString tag;
    QRect crop_rect;
    QString excel_col;

    QPoint first_press_pos;
    QPoint second_press_pos;

private:

};


class OCR_Format_Setting {
public:
    OCR_Format_Setting(QString setting_name, QImage image, std::vector<std::shared_ptr<OCR_Format_Data>>);
    ~OCR_Format_Setting();
    bool save_file(QString save_path);
private:
    QString settting_name;
    QImage image;
    std::vector<std::shared_ptr<OCR_Format_Data>> format_list;
};


#endif //CUSTOMIZE_OCR_OCR_FORMAT_STRUCT_H
