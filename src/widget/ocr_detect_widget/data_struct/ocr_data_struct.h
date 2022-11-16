//
// Created by idk on 2022/6/13.
//

#ifndef CUSTOMIZE_OCR_OCR_DATA_STRUCT_H
#define CUSTOMIZE_OCR_OCR_DATA_STRUCT_H


#include <QString>
#include <QPixmap>

struct OCR_Display_Data {
    QString tag;
    QPixmap crop_image;
    QString ocr_text;
};

#endif //CUSTOMIZE_OCR_OCR_DATA_STRUCT_H
