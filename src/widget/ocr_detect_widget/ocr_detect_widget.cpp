//
// Created by idk on 2022/6/10.
//

#include "ocr_detect_widget.h"
#include "ui_ocr_detect_widget.h"
#include "widget_item//ocr_data_widget.h"


OCR_Detect_Widget::OCR_Detect_Widget(QWidget *parent): QWidget(parent), ui(new Ui::OCR_Detect_Widget)
{
    this->ui->setupUi(this);




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
};


