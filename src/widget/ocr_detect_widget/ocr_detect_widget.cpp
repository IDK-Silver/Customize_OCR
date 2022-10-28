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

    connect(this->ui->pb_load_setting_file, SIGNAL(clicked()), SLOT(load_setting_file()));
    connect(this->ui->pb_star_ocr, SIGNAL(clicked()), SLOT(ocr_image()));
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
}

void OCR_Detect_Widget::load_setting_file() {
    OCR_Format_Setting setting;
    auto choose_file = QFileDialog::getOpenFileName(this, QString("選設定檔"),
                                                    QString::fromStdString(Config::Files_Path::get_format_setting_folder_path()),
                                                    QString("設定檔 (*.json)"));
    setting.load_file(choose_file);
    this->ocr_format_data_list = setting.get_format_list();
}

void OCR_Detect_Widget::ocr_image() {

    if (this->wait_ocr_path_list.empty()) {
        return;
    }

    QImage ocr_image(this->wait_ocr_path_list.at(0));


}






