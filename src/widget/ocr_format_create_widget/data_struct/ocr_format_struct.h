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

    struct JsonDict {
        QString setting_name_key = "setting_name";
        QString image_data_key = "image_data";
        struct Struct_data {
            QString key = "format_data";
            QString tag_name_key = "tag";
            QString excel_key = "excel";
        };
        Struct_data format_data;
    };
public:
    JsonDict json_dict;

    OCR_Format_Setting(QImage image, std::vector<std::shared_ptr<OCR_Format_Data>>);
    OCR_Format_Setting();
    ~OCR_Format_Setting();
    bool save_file(const QString &file_path);
    void load_file(const QString &file_path);
private:
    QString setting_name;
    QImage image;
    std::vector<std::shared_ptr<OCR_Format_Data>> format_list;
};


#endif //CUSTOMIZE_OCR_OCR_FORMAT_STRUCT_H
