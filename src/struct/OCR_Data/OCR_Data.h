//
// Created by idk on 2022/11/23.
//

#ifndef CUSTOMIZE_OCR_OCR_DATA_H
#define CUSTOMIZE_OCR_OCR_DATA_H

#include <QString>
#include <QRect>
#include <QPoint>
#include <QImage>
#include "src/widget/ocr_format_create_widget/data_struct/ocr_format_struct.h"

struct  BASIC_OCR_DATA_{
    QString tag;
    QRect crop_rect;
    QString xlsx_cell;
    QPoint first_press_pos;
    QPoint second_press_pos;
};


class OCR_Data_Json_Reader {
    struct JsonDict {
        QString setting_name_key = "setting_name";
        QString image_data_key = "image_data";
        struct Struct_data {
            QString key = "format_data";
            QString tag_name_key = "tag";
            QString excel_key = "excel";
            QString crop_image_rect_key = "image_rect";
        };
        Struct_data format_data;
    };
public:
    OCR_Data_Json_Reader(QImage image, std::vector<std::shared_ptr<OCR_Format_Data>>);
    OCR_Data_Json_Reader();
    ~OCR_Data_Json_Reader();
    bool save_file(const QString &file_path);
    void load_file(const QString &file_path);
    std::vector<std::shared_ptr<OCR_Format_Data>> get_format_list();
    QImage get_image();

private:
    QString setting_name;
    QImage image;
    std::vector<std::shared_ptr<BASIC_OCR_DATA_>> format_list;
};



class OCR_Data {



public:
    OCR_Data() = default;
    ~OCR_Data() = default;

};


#endif //CUSTOMIZE_OCR_OCR_DATA_H
