//
// Created by idk on 2022/6/10.
//

#include <QFileDialog>
#include "ocr_detect_widget.h"
#include "ui_ocr_detect_widget.h"
#include "widget_item//ocr_data_widget.h"
#include "lib/Config/Files_Path/Files_Path.h"


OCR_Detect_Widget::OCR_Detect_Widget(QWidget *parent): QWidget(parent), ui(new Ui::OCR_Detect_Widget)
{
    this->ui->setupUi(this);
    this->connect_setup();
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
}

void OCR_Detect_Widget::load_setting_file() {
    OCR_Format_Setting setting;

    /* get the choose file path */
    auto choose_file = QFileDialog::getOpenFileName(this, QString("選設定檔"),
                                                    QString::fromStdString(Config::Files_Path::get_format_setting_folder_path()),
                                                    QString("設定檔 (*.json)"));
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
    QImage ocr_image(this->wait_ocr_path_list.at(0));

    /* del the front path */
    this->wait_ocr_path_list.pop_front();

    /* image ocr */



    /* add list widget element (the ocr data)  */
    for (const auto &format_data : this->ocr_format_data_list)
    {
        OCR_Display_Data data;
        data.tag =  format_data->tag;
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

    /* to local string (to fix windows format) */
    for (auto &path : choose_files) {
        path = path.toLocal8Bit();
    }

    this->wait_ocr_path_list = choose_files;
}

