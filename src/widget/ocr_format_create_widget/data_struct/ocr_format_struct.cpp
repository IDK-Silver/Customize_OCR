//
// Created by idk on 2022/8/22.
//

#include "ocr_format_struct.h"

#include <utility>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QBuffer>

OCR_Format_Setting::OCR_Format_Setting(QImage image, std::vector<std::shared_ptr<OCR_Format_Data>> format_list) {
    this->image = std::move(image);
    this->format_list = std::move(format_list);
}

bool OCR_Format_Setting::save_file(const QString& file_path) {

    QDir path (this->setting_name);


    // QImage to QByteArray
    QByteArray image_data;
    {
        QBuffer buffer(&image_data);
        buffer.open(QIODevice::WriteOnly);
        image.save(&buffer, "PNG");
        buffer.close();
    }
    image_data = image_data.toBase64();

    QJsonObject json_obj;
    json_obj.insert(this->json_dict.setting_name_key, path.dirName());

    json_obj.insert(this->json_dict.image_data_key, QJsonValue::fromVariant(image_data));

    QJsonArray format_data_array;
    for (const auto& format : format_list) {
        QJsonObject format_data;
        format_data.insert(this->json_dict.format_data.tag_name_key, format->tag);
        format_data.insert(this->json_dict.format_data.excel_key, format->excel_col);

        // Image Crop Rect
        {
            QRect rect = format->crop_rect;
            QJsonArray image_crop_rect;
            image_crop_rect.push_back(rect.topLeft().x());
            image_crop_rect.push_back(rect.topLeft().y());
            image_crop_rect.push_back(rect.bottomRight().x());
            image_crop_rect.push_back(rect.bottomRight().y());
            format_data.insert(this->json_dict.format_data.crop_image_rect_key, image_crop_rect);
        }


        format_data_array.push_back(format_data);
    }

    json_obj.insert(this->json_dict.format_data.key, format_data_array);

    {
        QJsonDocument json_doc(json_obj);
        QByteArray byte_array = json_doc.toJson(QJsonDocument::Compact);
        QString json_str(byte_array);

        QFile file(file_path);

        if(!file.open(QIODevice::ReadWrite|QIODevice::Text)){
            qDebug() << "file error!";
            file.close();
            return false;
        }

        QTextStream in(&file);
        in << json_str;

        file.close();
    }
    return true;
}

void OCR_Format_Setting::load_file(const QString &file_path) {

    /* load json data string */
    QString json_str;
    {
        QFile file(file_path);
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        json_str = file.readAll();
        file.close();
    }

    /* decode json string to json doc */
    QJsonDocument json_doc = QJsonDocument::fromJson(json_str.toUtf8());

    /* get ocr image from json doc */
    auto byte_array_image_64 = json_doc[this->json_dict.image_data_key].toVariant().toByteArray();
    auto byte_array_image = QByteArray::fromBase64(byte_array_image_64);
    this->image.loadFromData(byte_array_image);

    /* clear old data and then load new data from load json file */
    this->format_list.clear();

    /* get ocr format data from json doc */
    /* each element in array is a tag data
     * tag data is (tag name, excel col, image need crop range)
     * */
    QJsonArray format_data_array = json_doc[this->json_dict.format_data.key].toArray();

    /* for load each tag format form array */
    for (auto format : format_data_array) {

        auto format_data = std::make_shared<OCR_Format_Data>();

        /* set data */
        format_data->tag = format[this->json_dict.format_data.tag_name_key].toString();
        format_data->excel_col = format[this->json_dict.format_data.excel_key].toString();

        // is part is image get the image crop range (QPoint to int)
        {
            auto point_array = format[this->json_dict.format_data.crop_image_rect_key].toArray();
            QRect rect(QPoint(point_array.at(0).toInt(), point_array.at(1).toInt()),
                       QPoint(point_array.at(2).toInt(), point_array.at(3).toInt()));
            format_data->crop_rect = rect;
        }

        format_list.push_back(format_data);
    }

}

std::vector<std::shared_ptr<OCR_Format_Data>> OCR_Format_Setting::get_format_list() {
    return this->format_list;
}

QImage OCR_Format_Setting::get_image() {
    return this->image;
}

OCR_Format_Setting::OCR_Format_Setting() = default;

OCR_Format_Setting::~OCR_Format_Setting() = default;
