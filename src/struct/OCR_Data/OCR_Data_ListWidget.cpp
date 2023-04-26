//
// Created by idk on 2022/11/23.
//

#include "OCR_Data_ListWidget.h"
#include "ui_ocr_widget_item.h"
#include <utility>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QBuffer>

OCR_Data_List::OCR_Data_List(QImage image, std::vector<std::shared_ptr<OCR_Data>> format_list) {
    this->image = std::move(image);
    this->format_list = std::move(format_list);
}

bool OCR_Data_List::save_file(const QString& file_path) {

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
        format_data.insert(this->json_dict.format_data.excel_key, format->xlsx_cell);
        format_data.insert(this->json_dict.format_data.is_search_key, format->is_search);
        format_data.insert(this->json_dict.format_data.search_weight_key, format->search_weight);
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

void OCR_Data_List::load_file(const QString &file_path) {

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
    /* each element in array is a tag data     *  is (tag name, xlsx_cell, image need crop range)
     * */
    QJsonArray format_data_array = json_doc[this->json_dict.format_data.key].toArray();

    /* for load each tag format form array */
    for (auto format : format_data_array) {

        auto format_data = std::make_shared<OCR_Data>();

        /* set data */
        format_data->tag = format[this->json_dict.format_data.tag_name_key].toString();
        format_data->xlsx_cell = format[this->json_dict.format_data.excel_key].toString();
        format_data->is_search = format[this->json_dict.format_data.is_search_key].toBool();
        format_data->search_weight = format[this->json_dict.format_data.search_weight_key].toInt();


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

std::vector<std::shared_ptr<OCR_Data>> OCR_Data_List::get_format_list() {
    return this->format_list;
}

QImage OCR_Data_List::get_image() {
    return this->image;
}

std::shared_ptr<OCR_Data> & OCR_Data_List::at(const unsigned int &index) {
    return this->format_list.at(index);
}

void OCR_Data_List::push_back(const std::shared_ptr<OCR_Data>& data) {
    this->format_list.push_back(data);
}

size_t OCR_Data_List::size() {
    return this->format_list.size();
}

bool OCR_Data_List::empty() {
    return this->format_list.empty();
}

void OCR_Data_List::clear() {
    this->format_list.clear();
}

int OCR_Data_List::distance(const std::shared_ptr<OCR_Data> &data) {

    int index = 0;
    for (const auto & iter : format_list)
    {
        if (iter->tag == data->tag)
            return index;
        index++;
    }

//    auto itr = std::find(this->format_list.begin(), this->format_list.end(), data);
//
//    if (itr != this->format_list.end())
//        return std::distance(this->format_list.begin(), itr);

    return 0;
}


OCR_Data_ListWidget::OCR_Data_ListWidget(QWidget *parent) : QListWidget(parent) {

}

void OCR_Data_ListWidget::add_item_widget(const std::shared_ptr<OCR_Data>& data) {

    // create widget and add to widget_item_array
    auto list_widget_item = std::make_shared<OCR_Data_ListItem>(this);
    this->list_item_vector.push_back(list_widget_item);
    list_widget_item->copy_from(data);

    // create QtListWidgetItem
    auto item = new QListWidgetItem();

    // set list widget view mode
    this->setViewMode(QListView::ListMode);

    // set widget height size
    item->setSizeHint(QSize(0, 100));
    this->addItem(item);
    this->setItemWidget(item, list_widget_item.get());

    // refresh
    list_widget_item->refresh_widget_data();

}

void OCR_Data_ListWidget::clear_widget() {
    this->clear();
    this->list_item_vector.clear();
}

size_t OCR_Data_ListWidget::size() {
    return this->list_item_vector.size();
}

std::shared_ptr<OCR_Data_ListItem> OCR_Data_ListWidget::at(size_t index) {
    return this->list_item_vector.at(index);
}

OCR_Data_ListItem::OCR_Data_ListItem(QWidget *parent) : QWidget(parent), ui(new Ui::OCR_Data_Item)
{
    ui->setupUi(this);
}

OCR_Data_ListItem::~OCR_Data_ListItem()
{
    delete ui;
}

void OCR_Data_ListItem::refresh_widget_data() {
    this->ui->tag_lab->setText(this->ocr_data->tag);
    this->ui->xlsx_cell_lab->setText(this->ocr_data->xlsx_cell);
    this->ui->ocr_text_lineEdit->setText(this->ocr_data->ocr_text);
    this->ui->crop_image_lab->setPixmap(this->ocr_data->crop_image);
}

void OCR_Data_ListItem::copy_from(const std::shared_ptr<OCR_Data>& data) {
    ocr_data = data;
}
