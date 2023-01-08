//
// Created by idk on 2022/11/23.
//

#ifndef CUSTOMIZE_OCR_OCR_DATA_LISTWIDGET_H
#define CUSTOMIZE_OCR_OCR_DATA_LISTWIDGET_H

#include <QString>
#include <QRect>
#include <QPoint>
#include <QImage>
#include <QWidget>
#include <QListWidget>
#include "ui_ocr_widget_item.h"


namespace Ui {
    class OCR_Data_Item;
}

struct OCR_Data {
    QString tag;
    QRect crop_rect;
    QString xlsx_cell;
    QString ocr_text;
    QPoint first_press_pos;
    QPoint second_press_pos;
    QPixmap crop_image;
};

class OCR_Data_ListItem : public QWidget {
    Q_OBJECT
public:

    explicit OCR_Data_ListItem(QWidget *parent = nullptr);
    void copy_from(const std::shared_ptr<OCR_Data>& data);
    ~OCR_Data_ListItem() override;
    std::shared_ptr<OCR_Data> ocr_data;
    void refresh_widget_data();
    Ui::OCR_Data_Item *ui;

private:

};



class OCR_Data_List {
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
    OCR_Data_List() = default;
    ~OCR_Data_List() = default;
    OCR_Data_List(QImage image, std::vector<std::shared_ptr<OCR_Data>>);

    std::shared_ptr<OCR_Data> at(const unsigned int &index);

    void push_back(const std::shared_ptr<OCR_Data>& data);

    bool save_file(const QString &file_path);
    void load_file(const QString &file_path);
    std::vector<std::shared_ptr<OCR_Data>> get_format_list();
    QImage get_image();

    size_t size();

    bool empty();

    void clear();

private:
    JsonDict json_dict;
    QString setting_name;
    QImage image;
    std::vector<std::shared_ptr<OCR_Data>> format_list;
};



class OCR_Data_ListWidget : public QListWidget {

public:
    explicit OCR_Data_ListWidget(QWidget *parent = nullptr);
    ~OCR_Data_ListWidget() override = default;
    void add_item_widget(const std::shared_ptr<OCR_Data>& data);
    void clear_widget();

    size_t size();

    std::shared_ptr<OCR_Data_ListItem> at(size_t index);
private:
    std::vector<std::shared_ptr<OCR_Data_ListItem>> list_item_vector;
};


#endif //CUSTOMIZE_OCR_OCR_DATA_LISTWIDGET_H
