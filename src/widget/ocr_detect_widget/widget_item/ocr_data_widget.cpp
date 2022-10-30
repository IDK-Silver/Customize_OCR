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

void OCR_Data_Widget::change_tag(const QString &text) {
    this->ui->tag_lab->setText(text);
}

void OCR_Data_Widget::change_crop_image(const QImage &crop_image) {
    this->ui->crop_image_lab->setPixmap(QPixmap::fromImage(crop_image));
}

void OCR_Data_Widget::change_ocr_text(const QString &text) {
    this->ui->ocr_text_lineEdit->setText(text);
}
