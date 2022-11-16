//
// Created by idk on 2022/6/10.
//

#include <QFileDialog>
#include <Utility/ImageFormat/ImageFormat.h>
#include "ocr_detect_widget.h"
#include "ui_ocr_detect_widget.h"
#include <widget_item//ocr_data_widget.h>
#include <Config/Files_Path/Files_Path.h>


OCR_Detect_Widget::OCR_Detect_Widget(QWidget *parent): QWidget(parent), ui(new Ui::OCR_Detect_Widget)
{
    this->ui->setupUi(this);
    this->connect_setup();
    this->ocr_processing = std::make_unique<ImageOCR>();
}

OCR_Detect_Widget::~OCR_Detect_Widget() = default;


void OCR_Detect_Widget::add_list_widget_ocr_data(const OCR_Display_Data& data) {

    // create widget and add to list_widget
    auto ocr_data_widget = new OCR_Data_Widget(this);
    auto item = new QListWidgetItem();
    ui->listWidget->setViewMode(QListView::ListMode);

    // set widget height size
    item->setSizeHint(QSize(0, 100));
    this->ui->listWidget->addItem(item);
    this->ui->listWidget->setItemWidget(item, ocr_data_widget);

    // change widget data
    ocr_data_widget->change_tag(data.tag);
    ocr_data_widget->change_crop_image(data.crop_image);
    ocr_data_widget->change_ocr_text(data.ocr_text);
}

void OCR_Detect_Widget::load_setting_file() {
    OCR_Format_Setting setting;

    /* get the choose file path */
    auto choose_file = QFileDialog::getOpenFileName(this, QString("選設定檔"),
                                                    QString::fromStdString(Config::Files_Path::get_format_setting_folder_path()),
                                                    QString("設定檔 (*.json)")).toLocal8Bit();
    /* load the setting file*/
    setting.load_file(choose_file);

    /* get the setting dates */
    this->ocr_format_data_list = setting.get_format_list();

}

void OCR_Detect_Widget::ocr_image() {

    /* clear the list widget element */
    this->ui->listWidget->clear();


    /* check is image need ocr */
    if (this->wait_ocr_path_list.empty()) {
        return;
    }

    /* load image by the first index of wait ocr path list */
    cv::Mat  image = cv::imread(this->wait_ocr_path_list.at(0).toLocal8Bit().toStdString());

    /* del the front path */
    this->wait_ocr_path_list.pop_front();


    /* add list widget element (the ocr data)  */
    /* set OCR display data from OCR Format Data
     * and add to the list widget to display in UI
     * */
    for (const auto &format_data : this->ocr_format_data_list)
    {
        OCR_Display_Data data;

        /* crop image by format crop rect*/
        cv::Mat crop_image = Utility::MatCropByQRect(image, format_data->crop_rect);

        ocr_processing->set_image(crop_image);

        /* set OCR display data */
        data.tag =  format_data->tag;
        data.ocr_text = this->ocr_processing->ocr();
        data.crop_image = Utility::Mat2QPixmap(crop_image);
        this->add_list_widget_ocr_data(data);
    }


}

void OCR_Detect_Widget::action_open_file() {
    load_setting_file();
}


void OCR_Detect_Widget::connect_setup() {
    connect(this->ui->pb_load_setting_file, SIGNAL(clicked()), SLOT(load_setting_file()));
    connect(this->ui->pb_star_ocr, SIGNAL(clicked()), SLOT(ocr_image()));
    connect(this->ui->pb_choose_image, SIGNAL(clicked()), SLOT(choose_images()));
}

void OCR_Detect_Widget::choose_images() {
    QStringList choose_files = QFileDialog::getOpenFileNames(this, QString("選擇圖片"),
                                                    QString::fromStdString(Config::Files_Path::get_system_picture_path()),
                                                    QString("圖片 (*.jpg *.png *tif)"));

    this->wait_ocr_path_list = choose_files;
}

