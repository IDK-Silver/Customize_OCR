#include "ocr_data_widget.h"
#include "ui_ocr_data_widget.h"

OCR_Data_Widget::OCR_Data_Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OCR_Data_Widget)
{
    ui->setupUi(this);
}

OCR_Data_Widget::~OCR_Data_Widget()
{
    delete ui;
}

void OCR_Data_Widget::change_lable() {
    this->ui->label->setText("Test");
}
